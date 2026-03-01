#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "camera.h"
#include "renderer.h"
#include "renderers/flat_schwarzschild.h"
#include "sim_params.h"
#include "vec3.h"

// ============================================================
//  LAYOUT
// ============================================================
const int WIN_W     = 1200;
const int WIN_H     = 750;
const int SIDEBAR_W = 350;
const int VIEW_W    = WIN_W - SIDEBAR_W;
const int VIEW_H    = WIN_H;
const int LO_W      = VIEW_W / 3;
const int LO_H      = VIEW_H / 3;

// ============================================================
//  CAMERA STATE
//
//  Spherical coords:
//    distance : how far from the origin (zoom)
//    theta    : vertical angle (elevation), in radians
//    phi      : horizontal angle (azimuth), in radians
// ============================================================
struct CameraState {
  double distance = 25.0;
  double theta    = 0.18;
  double phi      = 0.0;
  double fov      = 40.0;

  // Convert spherical coords to a cartesian camera position
  Vec3 position() const { return Vec3(distance * std::cos(theta) * std::sin(phi), distance * std::sin(theta), distance * std::cos(theta) * std::cos(phi)); }
};

// ============================================================
//  RENDERING
//  Renders into a flat array of sf::Uint8 (R,G,B,A per pixel).
//  renderWidth/Height can be smaller than the window for fast
//  preview rendering — SFML will stretch it to fill the window.
// ============================================================
void renderToBuffer(std::vector<std::uint8_t> &pixels, int rW, int rH, const CameraState &cam, const Renderer &renderer, const SimParams &params) {
  double aspect = static_cast<double>(rW) / rH;
  Vec3 pos      = cam.position();
  Camera camera(pos, Vec3(0, 0, 0), Vec3(0, 1, 0), cam.fov, aspect);

  for (int j = 0; j < rH; ++j) {
    for (int i = 0; i < rW; ++i) {
      double s = (i + 0.5) / rW;
      double t = (j + 0.5) / rH;
      Vec3 dir = camera.getRayDirection(s, t);
      Color c  = renderer.traceRay(pos, dir, params);

      // Gamma correction
      c.x = std::pow(std::clamp(c.x, 0.0, 1.0), 1.0 / 2.2);
      c.y = std::pow(std::clamp(c.y, 0.0, 1.0), 1.0 / 2.2);
      c.z = std::pow(std::clamp(c.z, 0.0, 1.0), 1.0 / 2.2);

      int flippedJ = (rH - 1 - j);
      int idx      = (flippedJ * rW + i) * 4;

      pixels[idx + 0] = static_cast<std::uint8_t>(255.99 * c.x);
      pixels[idx + 1] = static_cast<std::uint8_t>(255.99 * c.y);
      pixels[idx + 2] = static_cast<std::uint8_t>(255.99 * c.z);
      pixels[idx + 3] = 255;
    }
  }
}

// ============================================================
//  SAVE PNG
// ============================================================
void savePNG(const CameraState &cam, const Renderer &renderer, const SimParams &params, int w, int h) {
  std::cout << "Saving PNG at " << w << "x" << h << "...\n";
  std::vector<std::uint8_t> px(w * h * 4);
  renderToBuffer(px, w, h, cam, renderer, params);
  sf::Image img(sf::Vector2u(w, h), px.data());
  if (img.saveToFile("blackhole_hires.png"))
    std::cout << "Saved blackhole_hires.png\n";
  else
    std::cerr << "Failed to save PNG\n";
}

// ============================================================
//  MAIN
// ============================================================
int main() {
  std::vector<std::unique_ptr<Renderer>> renderers;
  renderers.push_back(std::make_unique<FlatSchwarzschildRenderer>());

  int activeRenderer = 0;
  SimParams params;
  CameraState camState;

  sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WIN_W, WIN_H)), sf::String(L"Black Hole Simulator"));
  window.setFramerateLimit(60);

  if (!ImGui::SFML::Init(window)) {
    std::cerr << "Failed to init ImGui-SFML\n";
    return 1;
  }

  ImGui::StyleColorsDark();
  ImGuiStyle &style    = ImGui::GetStyle();
  style.WindowRounding = 0.0f;
  style.FrameRounding  = 3.0f;
  style.ItemSpacing    = ImVec2(8, 6);

  sf::Texture hiTex;
  hiTex.resize(sf::Vector2u(VIEW_W, VIEW_H));
  sf::Sprite hiSprite(hiTex);
  hiSprite.setPosition(sf::Vector2f(SIDEBAR_W, 0));

  sf::Texture loTex;

  std::vector<std::uint8_t> hiPixels(VIEW_W * VIEW_H * 4);
  std::vector<std::uint8_t> loPixels(LO_W * LO_H * 4);

  bool isDragging    = false;
  int lastMouseX     = 0;
  int lastMouseY     = 0;
  bool needsHiRender = true;
  bool showingLowRes = false;
  bool paramsChanged = false;

  std::cout << "Initial render...\n";
  renderToBuffer(hiPixels, VIEW_W, VIEW_H, camState, *renderers[activeRenderer], params);
  hiTex.update(hiPixels.data(), sf::Vector2u(VIEW_W, VIEW_H), sf::Vector2u(0, 0));

  sf::Clock deltaClock;

  std::vector<std::string> rendererNames;
  std::vector<const char *> rendererNamePtrs;
  for (auto &r : renderers) {
    rendererNames.push_back(r->name());
    rendererNamePtrs.push_back(rendererNames.back().c_str());
  }

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);

      if (event->is<sf::Event::Closed>())
        window.close();

      if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Escape)
          window.close();
        if (key->code == sf::Keyboard::Key::S)
          savePNG(camState, *renderers[activeRenderer], params, VIEW_W * 2, VIEW_H * 2);
      }

      auto mouseInViewport = [](int x) { return x >= SIDEBAR_W; };

      if (const auto *btn = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (btn->button == sf::Mouse::Button::Left && mouseInViewport(btn->position.x) && !ImGui::GetIO().WantCaptureMouse) {
          isDragging = true;
          lastMouseX = btn->position.x;
          lastMouseY = btn->position.y;
        }
      }

      if (const auto *btn = event->getIf<sf::Event::MouseButtonReleased>()) {
        if (btn->button == sf::Mouse::Button::Left && isDragging) {
          isDragging    = false;
          needsHiRender = true;
        }
      }

      if (const auto *mv = event->getIf<sf::Event::MouseMoved>()) {
        if (isDragging) {
          int dx     = mv->position.x - lastMouseX;
          int dy     = mv->position.y - lastMouseY;
          lastMouseX = mv->position.x;
          lastMouseY = mv->position.y;

          camState.phi -= dx * 0.005;
          camState.theta = std::clamp(camState.theta + dy * 0.005, -1.4, 1.4);

          renderToBuffer(loPixels, LO_W, LO_H, camState, *renderers[activeRenderer], params);
          showingLowRes = true;
          needsHiRender = true;
        }
      }

      if (const auto *scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
        if (!ImGui::GetIO().WantCaptureMouse && mouseInViewport(static_cast<int>(scroll->position.x))) {
          camState.distance -= scroll->delta * 1.5;
          camState.distance = std::clamp(camState.distance, 14.0, 80.0);
          renderToBuffer(loPixels, LO_W, LO_H, camState, *renderers[activeRenderer], params);
          showingLowRes = true;
          needsHiRender = true;
        }
      }
    }

    // ---- ImGui sidebar ----
    ImGui::SFML::Update(window, deltaClock.restart());

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(SIDEBAR_W, WIN_H));
    ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::SeparatorText("Simulation");

    ImGui::Combo("##sim", &activeRenderer, rendererNamePtrs.data(), static_cast<int>(rendererNamePtrs.size()));

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.65f, 0.65f, 1.0f));
    ImGui::TextWrapped("%s", renderers[activeRenderer]->description().c_str());
    ImGui::PopStyleColor();
    ImGui::Spacing();

    ImGui::SeparatorText("Black Hole");
    if (ImGui::SliderFloat("Rs", &params.rs, 0.1f, 3.0f, "%.2f"))
      paramsChanged = true;
    ImGui::SetItemTooltip("Schwarzschild radius (unit of length)");
    ImGui::Spacing();

    ImGui::SeparatorText("Accretion Disk");
    if (ImGui::SliderFloat("Inner (x Rs)", &params.diskInnerMul, 1.5f, 6.0f, "%.1f")) {
      params.diskInnerMul = std::min(params.diskInnerMul, params.diskOuterMul - 1.0f);
      paramsChanged       = true;
    }
    ImGui::SetItemTooltip("Inner edge (ISCO = 3x Rs for Schwarzschild)");
    if (ImGui::SliderFloat("Outer (x Rs)", &params.diskOuterMul, 4.0f, 24.0f, "%.1f")) {
      params.diskOuterMul = std::max(params.diskOuterMul, params.diskInnerMul + 1.0f);
      paramsChanged       = true;
    }
    ImGui::Spacing();

    ImGui::SeparatorText("Camera");
    float dist  = static_cast<float>(camState.distance);
    float theta = static_cast<float>(camState.theta);
    float fov   = static_cast<float>(camState.fov);
    if (ImGui::SliderFloat("Distance", &dist, 14.0f, 80.0f, "%.1f")) {
      camState.distance = dist;
      paramsChanged     = true;
    }
    if (ImGui::SliderFloat("Elevation", &theta, -1.4f, 1.4f, "%.2f rad")) {
      camState.theta = theta;
      paramsChanged  = true;
    }
    if (ImGui::SliderFloat("FOV", &fov, 20.0f, 90.0f, "%.0f deg")) {
      camState.fov  = fov;
      paramsChanged = true;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Re-render", ImVec2(-1, 0)))
      paramsChanged = true;
    ImGui::Spacing();
    if (ImGui::Button("Save PNG (2x res)", ImVec2(-1, 0)))
      savePNG(camState, *renderers[activeRenderer], params, VIEW_W * 2, VIEW_H * 2);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.45f, 0.45f, 0.45f, 1.0f));
    ImGui::TextWrapped("Drag viewport to orbit.\nScroll to zoom.\nS key to save.");
    ImGui::PopStyleColor();

    ImGui::End();

    // ---- Trigger hi-res render ----
    if (paramsChanged && !isDragging) {
      renderToBuffer(loPixels, LO_W, LO_H, camState, *renderers[activeRenderer], params);
      showingLowRes = true;
      needsHiRender = true;
      paramsChanged = false;
    }

    if (needsHiRender && !isDragging && showingLowRes) {
      std::cout << "Full-res render (" << renderers[activeRenderer]->name() << ")...\n";
      renderToBuffer(hiPixels, VIEW_W, VIEW_H, camState, *renderers[activeRenderer], params);
      hiTex.update(hiPixels.data(), sf::Vector2u(VIEW_W, VIEW_H), sf::Vector2u(0, 0));
      needsHiRender = false;
      showingLowRes = false;
    }

    // ---- Draw ----
    window.clear(sf::Color(20, 20, 20));
    if (showingLowRes) {
      loTex.resize(sf::Vector2u(LO_W, LO_H));
      loTex.update(loPixels.data(), sf::Vector2u(LO_W, LO_H), sf::Vector2u(0, 0));
      sf::Sprite loSprite(loTex);
      loSprite.setPosition(sf::Vector2f(SIDEBAR_W, 0));
      loSprite.setScale(sf::Vector2f(static_cast<float>(VIEW_W) / LO_W, static_cast<float>(VIEW_H) / LO_H));
      window.draw(loSprite);
    } else {
      window.draw(hiSprite);
    }

    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}