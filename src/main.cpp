#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "camera.h"
#include "vec3.h"

// ============================================================
//  SCENE PARAMETERS
//  All distances are in units of the Schwarzschild radius (Rs).
//  Rs = 2GM/c^2. We set Rs = 1 for simplicity.
//  In Schwarzschild coordinates, the event horizon is at r = 1 Rs.
// ============================================================
const double RS            = 1.0; // Schwarzschild radius (our unit of length)
const double EVENT_HORIZON = RS;  // r at which light cannot escape
// const double PHOTON_SPHERE  = 1.5 * RS; // r = 1.5 Rs: unstable photon orbits live here
const double DISK_INNER = 3.0 * RS; // Innermost Stable Circular Orbit (ISCO) for Schwarzschild
const double DISK_OUTER = 12.0 * RS;
// const double DISK_THICKNESS = 0.05 * RS; // Thin disk half-thickness for intersection test

// ============================================================
//  COLOR HELPERS
// ============================================================

// Simple background: deep space (nearly black with a faint blue tint)
Color backgroundColor(const Vec3 &dir) {
  // Subtle gradient so we can tell the top from the bottom
  double t = 0.5 * (dir.normalized().y + 1.0);
  return (1.0 - t) * Color(0.0, 0.0, 0.0) + t * Color(0.02, 0.02, 0.08);
}

// Simple radial gradient for the accretion disk
Color diskColor(double r) {
  double t = (r - DISK_INNER) / (DISK_OUTER - DISK_INNER);
  t        = std::clamp(t, 0.0, 1.0);

  if (t < 0.2) {
    // White -> yellow
    double s = t / 0.2;
    return Color(1.0, 1.0, 1.0 - s);
  } else if (t < 0.5) {
    // Yellow -> orange
    double s = (t - 0.2) / 0.3;
    return Color(1.0, 1.0 - 0.4 * s, 0.0);
  } else {
    // Orange -> dim red
    double s = (t - 0.5) / 0.5;
    return Color(1.0 - 0.6 * s, 0.6 - 0.5 * s, 0.0);
  }
}

// ============================================================
//  HIT TESTS
// ============================================================

struct HitResult {
  bool hit = false;
  double t = 0.0; // Parameter along the ray where the hit occurred
  double r = 0.0; // Radial distance from the black hole at hit point
};

// Test if the ray hits the event horizon (a sphere of radius RS centered at the origin).
HitResult hitEventHorizon(const Vec3 &origin, const Vec3 &dir) {
  double a       = Vec3::dot(dir, dir);
  double b       = 2.0 * Vec3::dot(origin, dir);
  double c       = Vec3::dot(origin, origin) - EVENT_HORIZON * EVENT_HORIZON;
  double discrim = b * b - 4 * a * c;

  if (discrim < 0)
    return {}; // No intersection

  double t = (-b - std::sqrt(discrim)) / (2.0 * a); // Nearest intersection
  if (t < 0)
    return {}; // Behind the camera

  return {true, t, EVENT_HORIZON};
}

// Test if the ray hits the accretion disk
HitResult hitDisk(const Vec3 &origin, const Vec3 &dir) {
  if (std::abs(dir.y) < 1e-8)
    return {}; // Ray is parallel to disk plane

  double t = -origin.y / dir.y;
  if (t < 0.001)
    return {}; // Hit is behind or too close to camera

  Vec3 p   = origin + t * dir;
  double r = std::sqrt(p.x * p.x + p.z * p.z); // Radial distance in xz plane

  if (r < DISK_INNER || r > DISK_OUTER)
    return {}; // Outside the annulus

  return {true, t, r};
}

Color traceRay(const Vec3 &origin, const Vec3 &dir) {
  HitResult horizonHit = hitEventHorizon(origin, dir);
  HitResult diskHit    = hitDisk(origin, dir);

  // If we hit the disk AND the disk is closer than the event horizon
  if (diskHit.hit) {
    bool blockedByHorizon = horizonHit.hit && horizonHit.t < diskHit.t;
    if (!blockedByHorizon) {
      double cosAngle   = std::abs(dir.y / dir.length());
      double brightness = 0.6 + 0.4 * cosAngle;
      return diskColor(diskHit.r); // * brightness;
    }
  }

  // If we hit the event horizon (and nothing closer was hit), render it black
  if (horizonHit.hit) {
    return Color(0.0, 0.0, 0.0);
  }

  // Nothing hit — render the background
  return backgroundColor(dir);
}

// ============================================================
//  PPM IMAGE OUTPUT
// ============================================================

void writePPM(const std::string &filename, const std::vector<std::vector<Color>> &pixels, int width, int height) {
  std::ofstream file(filename);
  file << "P3\n" << width << " " << height << "\n255\n";

  // PPM stores top row first, so we iterate y from high to low
  for (int j = height - 1; j >= 0; --j) {
    for (int i = 0; i < width; ++i) {
      Color c = pixels[j][i];
      // Gamma correction: raise to power 1/2.2 so the image looks right on screen
      c.x   = std::pow(std::clamp(c.x, 0.0, 1.0), 1.0 / 2.2);
      c.y   = std::pow(std::clamp(c.y, 0.0, 1.0), 1.0 / 2.2);
      c.z   = std::pow(std::clamp(c.z, 0.0, 1.0), 1.0 / 2.2);
      int r = static_cast<int>(255.99 * c.x);
      int g = static_cast<int>(255.99 * c.y);
      int b = static_cast<int>(255.99 * c.z);
      file << r << " " << g << " " << b << "\n";
    }
  }
  std::cout << "Wrote " << filename << "\n";
}

int main() {
  // Image parameters
  const int WIDTH     = 800;
  const int HEIGHT    = 600;
  const double ASPECT = static_cast<double>(WIDTH) / HEIGHT;

  // Camera setup
  Vec3 origin(0.0, 4.0 * RS, 25.0 * RS);
  Vec3 target(0.0, 0.0, 0.0);
  Vec3 up(0.0, 1.0, 0.0);
  double fov = 40.0;

  Camera camera(origin, target, up, fov, ASPECT);

  // Pixel buffer
  std::vector<std::vector<Color>> pixels(HEIGHT, std::vector<Color>(WIDTH));

  // Render loop
  std::cout << "Rendering " << WIDTH << "x" << HEIGHT << "...\n";
  for (int j = 0; j < HEIGHT; j++) {
    if (j % (HEIGHT / 10) == 0)
      std::cout << (100 * j / HEIGHT) << "%\n";

    for (int i = 0; i < WIDTH; i++) {
      double s = (i + 0.5) / WIDTH;
      double t = (j + 0.5) / HEIGHT;

      Vec3 rayDir  = camera.getRayDirection(s, t);
      pixels[j][i] = traceRay(origin, rayDir);
    }
  }

  writePPM("output.ppm", pixels, WIDTH, HEIGHT);
  std::cout << "Done! Open output.ppm to view the result.\n";
  return 0;
}