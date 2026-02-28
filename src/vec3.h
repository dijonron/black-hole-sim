#pragma once
#include <cmath>
#include <iostream>

// A 3D vector class used for positions, directions, and colors.
struct Vec3
{
  double x, y, z;

  // Constructors
  Vec3() : x(0), y(0), z(0) {}
  Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

  /*
   * Operator overloads
   */
  // Vector addition, subtraction, scalar multiplication, and division
  Vec3 operator+(const Vec3 &v) const { return {x + v.x, y + v.y, z + v.z}; }
  Vec3 operator-(const Vec3 &v) const { return {x - v.x, y - v.y, z - v.z}; }
  Vec3 operator*(double t) const { return {x * t, y * t, z * t}; }
  Vec3 operator/(double t) const { return {x / t, y / t, z / t}; }

  // Unary negation
  Vec3 operator-() const { return {-x, -y, -z}; }

  // Component-wise multiply
  Vec3 operator*(const Vec3 &v) const { return {x * v.x, y * v.y, z * v.z}; }

  // Compound assignment
  Vec3 &operator+=(const Vec3 &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
  Vec3 &operator*=(double t)
  {
    x *= t;
    y *= t;
    z *= t;
    return *this;
  }

  /*
   * Common vector operations
   */
  double lengthSquared() const { return x * x + y * y + z * z; }

  double length() const { return std::sqrt(lengthSquared()); }

  Vec3 normalized() const { return *this / length(); }

  static double dot(const Vec3 &a, const Vec3 &b)
  {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }

  static Vec3 cross(const Vec3 &a, const Vec3 &b)
  {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x};
  }
  void print() const
  {
    std::cout << "(" << x << ", " << y << ", " << z << ")\n";
  }
};

// Allow: 3.0 * v  (scalar on the left)
inline Vec3 operator*(double t, const Vec3 &v) { return v * t; }

// Type alias: Color is just a Vec3 where x=R, y=G, z=B in range [0, 1]
using Color = Vec3;