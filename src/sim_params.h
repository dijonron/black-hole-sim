#pragma once

// ============================================================
//  SimParams
//  All parameters that can be changed via the UI at runtime.
//  Passed by const-ref into every renderer so they stay stateless.
// ============================================================
struct SimParams {
  // --- Black hole ---
  float rs = 1.0f; // Schwarzschild radius (unit of length)

  // --- Accretion disk ---
  float diskInnerMul = 1.5f; // Inner edge as a multiple of rs (ISCO = 3rs for Schwarzschild)
  float diskOuterMul = 5.0f; // Outer edge as a multiple of rs
  bool showDisk      = true;

  // --- Derived (computed from the above) ---
  double eventHorizon() const { return rs; }
  double diskInner() const { return rs * diskInnerMul; }
  double diskOuter() const { return rs * diskOuterMul; }
};
