#ifndef UTILS_HPP
#define UTILS_HPP
#include "vector.hpp"

#define EPSILON 1e-5

struct Color {
  double r, g, b;

  Color operator*(const double& scaleFactor) const {
    return {r * scaleFactor, g * scaleFactor, b * scaleFactor};
  }

  Color operator+(const Color &otherColor) const {
    double clampedRed = std::min(1.0, r + otherColor.r);
    double clampedGreen = std::min(1.0, g + otherColor.g);
    double clampedBlue = std::min(1.0, b + otherColor.b);
    return {clampedRed, clampedGreen, clampedBlue};
  }
};

struct Light {
  Vector position;
  Vector lookAt;
  Color color;
  double fallOff;
  double cutOffAngle;
};

#endif