#pragma once
#include "vec3.h"

// Defines the viewpoint and generates one ray per pixel.
// We use a simple pinhole camera model, which means the camera is infinitely small and all rays originate from the same point (the "pinhole").
struct Camera {
  Vec3 origin;     // Where the camera is in the scene
  Vec3 lowerLeft;  // The bottom-left corner of the image plane
  Vec3 horizontal; // The full horizontal span of the image plane
  Vec3 vertical;   // The full vertical span of the image plane

  // Set up the camera
  //   lookFrom: The position of the camera in the scene.
  //   target: The point in the scene the camera is looking at.
  //   up: A vector that defines the "up" direction for the camera (usually (0, 1, 0)).
  //   vFov: Vertical field of view in degrees.
  //   aspectRatio: The width-to-height ratio of the image.
  Camera(Vec3 lookFrom, Vec3 target, Vec3 up, double vFov, double aspectRatio) {
    double theta      = vFov * M_PI / 180.0;      // Convert FOV to radians
    double halfHeight = std::tan(theta / 2.0);    // Calculate half the height of the image plane based on FOV
    double halfWidth  = aspectRatio * halfHeight; // Calculate half the width of the image plane based on aspect ratio

    origin = lookFrom;

    // Orthonormal basis for the camera
    Vec3 w = (lookFrom - target).normalized(); // Behind
    Vec3 u = Vec3::cross(up, w).normalized();  // Right
    Vec3 v = Vec3::cross(w, u);                // Up

    // Image plane setup
    lowerLeft  = origin - u * halfWidth - v * halfHeight - w;
    horizontal = u * (2.0 * halfWidth);
    vertical   = v * (2.0 * halfHeight);
  }

  // Generates a normalized ray direction from the camera through a point on the view plane.
  Vec3 getRayDirection(double s, double t) const {
    Vec3 target = lowerLeft + s * horizontal + t * vertical;
    return (target - origin).normalized();
  }
};