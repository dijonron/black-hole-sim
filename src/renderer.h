#pragma once
#include "sim_params.h"
#include "vec3.h"
#include <string>

// ============================================================
//  Renderer (abstract base class)
// ============================================================
class Renderer {
public:
  virtual ~Renderer() = default;

  // The name shown in the UI dropdown
  virtual std::string name() const = 0;

  // A short description shown in the UI
  virtual std::string description() const = 0;

  // Trace a single ray and return its color.
  // origin : camera position in world space
  // dir    : normalized ray direction
  // params : current simulation parameters
  virtual Color traceRay(const Vec3 &origin, const Vec3 &dir, const SimParams &params) const = 0;
};