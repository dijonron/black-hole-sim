#pragma once
#include "../renderer.h"
#include <algorithm>
#include <cmath>

// ============================================================
//  GRSchwarzschildRenderer
//
//  Numerically integrates null geodesics in Schwarzschild spacetime.
//  Rays travel in curved paths — producing real gravitational lensing,
//  the photon ring, and an accurate black hole shadow.
//
//  Coordinate system: equatorial plane (r, phi) only.
//  Every geodesic stays in a plane through the origin, so we rotate
//  each ray into that plane, integrate in 2D, then reconstruct 3D.
// ============================================================
class GRSchwarzschildRenderer : public Renderer {
public:
  std::string name() const override { return "Schwarzschild (GR Geodesics)"; }

  std::string description() const override {
    return "Null geodesics integrated via RK4 in Schwarzschild "
           "spacetime. Produces real gravitational lensing, "
           "photon ring, and accurate black hole shadow.";
  }

  Color traceRay(const Vec3 &origin, const Vec3 &dir, const SimParams &params) const override {
    // TODO
  }

private:
  Color diskColor(double r, const SimParams &p) const {
    double t = (r - p.diskInner()) / (p.diskOuter() - p.diskInner());
    t        = std::clamp(t, 0.0, 1.0);
    if (t < 0.2) {
      double s = t / 0.2;
      return Color(1.0, 1.0, 1.0 - s);
    } else if (t < 0.5) {
      double s = (t - 0.2) / 0.3;
      return Color(1.0, 1.0 - 0.4 * s, 0.0);
    } else {
      double s = (t - 0.5) / 0.5;
      return Color(1.0 - 0.6 * s, 0.6 - 0.5 * s, 0.0);
    }
  }

  Color backgroundColor(const Vec3 &dir) const {
    double t = 0.5 * (dir.normalized().y + 1.0);
    return (1.0 - t) * Color(0.0, 0.0, 0.0) + t * Color(0.02, 0.02, 0.08);
  }
};