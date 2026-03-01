#pragma once
#include "../renderer.h"
#include <algorithm>
#include <cmath>

// ============================================================
//  FlatSchwarzschildRenderer
//
//  Straight-line (Euclidean) ray tracing with a Schwarzschild
//  scene: event horizon sphere + flat annular accretion disk.
//  No actual general relativity — rays travel in straight lines.
// ============================================================
class FlatSchwarzschildRenderer : public Renderer {
public:
  std::string name() const override { return "Schwarzschild (Flat Space)"; }

  std::string description() const override { return "Straight rays, no lensing. Event horizon and disk are correct geometry, but light paths are Euclidean."; }

  Color traceRay(const Vec3 &origin, const Vec3 &dir, const SimParams &params) const override {
    HitResult horizon = hitHorizon(origin, dir, params.eventHorizon());
    HitResult disk    = hitDisk(origin, dir, params);

    // If we hit the disk AND the disk is closer than the event horizon
    if (disk.hit) {
      bool blocked = horizon.hit && horizon.t < disk.t;
      if (!blocked) {
        double cosAngle   = std::abs(dir.y / dir.length());
        double brightness = 0.6 + 0.4 * cosAngle;
        return diskColor(disk.r, params) * brightness;
      }
    }

    // If we hit the event horizon (and nothing closer was hit), render it black
    if (horizon.hit)
      return Color(0.0, 0.0, 0.0);
    return backgroundColor(dir);
  }

private:
  struct HitResult {
    bool hit = false;
    double t = 0.0;
    double r = 0.0;
  };

  // Test if the ray hits the event horizon (a sphere of radius RS centered at the origin).
  HitResult hitHorizon(const Vec3 &origin, const Vec3 &dir, double radius) const {
    double a    = Vec3::dot(dir, dir);
    double b    = 2.0 * Vec3::dot(origin, dir);
    double c    = Vec3::dot(origin, origin) - radius * radius;
    double disc = b * b - 4 * a * c;

    // No intersection
    if (disc < 0)
      return {};

    // Nearest intersection
    double t = (-b - std::sqrt(disc)) / (2.0 * a);

    // Behind the camera
    if (t < 0)
      return {};

    return {true, t, radius};
  }

  // Test if the ray hits the accretion disk
  HitResult hitDisk(const Vec3 &origin, const Vec3 &dir, const SimParams &p) const {
    // Ray is parallel to disk plane
    if (std::abs(dir.y) < 1e-8)
      return {};

    // Hit is behind or too close to camera
    double t = -origin.y / dir.y;
    if (t < 0.001)
      return {};

    Vec3 pt = origin + t * dir;
    // Radial distance in xz plane
    double r = std::sqrt(pt.x * pt.x + pt.z * pt.z);

    // Outside the annulus
    if (r < p.diskInner() || r > p.diskOuter())
      return {};

    return {true, t, r};
  }

  Color diskColor(double r, const SimParams &p) const {
    // Simple radial gradient for the accretion disk
    double t = (r - p.diskInner()) / (p.diskOuter() - p.diskInner());
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

  Color backgroundColor(const Vec3 &dir) const {
    double t = 0.5 * (dir.normalized().y + 1.0);
    return (1.0 - t) * Color(0.0, 0.0, 0.0) + t * Color(0.02, 0.02, 0.08);
  }
};