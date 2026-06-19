#ifndef CUBE_HPP
#define CUBE_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"
#include "square.hpp"


class Cube : public Object {
private:
  Vector originCorner;
  double edgeLength;  
  Square cubeFaces[6];
public:
  Cube(Vector cornerPosition, double sideLength, Color cubeColor, double ambientValue, double diffuseValue, double reflectionValue, double specularValue, double shininessValue) 
    : Object(cubeColor, ambientValue, diffuseValue, reflectionValue, specularValue, shininessValue),
      originCorner(cornerPosition), edgeLength(sideLength) {

    Vector topLowerRight(originCorner.x + edgeLength, originCorner.y, originCorner.z + edgeLength);
    Vector topUpperRight(originCorner.x + edgeLength, originCorner.y + edgeLength, originCorner.z + edgeLength);
    Vector topUpperLeft(originCorner.x, originCorner.y + edgeLength, originCorner.z + edgeLength);
    Vector bottomUpperRight(originCorner.x + edgeLength, originCorner.y + edgeLength, originCorner.z);

    cubeFaces[0] = Square(topUpperRight, Vector(1, 0, 0), edgeLength, cubeColor, ambientValue, diffuseValue, reflectionValue, specularValue, shininessValue);
    cubeFaces[1] = Square(topLowerRight, Vector(0, -1, 0), edgeLength, cubeColor, ambientValue, diffuseValue, reflectionValue, specularValue, shininessValue);
    cubeFaces[2] = Square(topUpperRight, Vector(0, 0, 1), edgeLength, cubeColor, ambientValue, diffuseValue, reflectionValue, specularValue, shininessValue);
    cubeFaces[3] = Square(topUpperRight, Vector(0, 1, 0), edgeLength, cubeColor, ambientValue, diffuseValue, reflectionValue, specularValue, shininessValue);
    cubeFaces[4] = Square(topUpperLeft, Vector(-1, 0, 0), edgeLength, cubeColor, ambientValue, diffuseValue, reflectionValue, specularValue, shininessValue);
    cubeFaces[5] = Square(bottomUpperRight, Vector(0, 0, -1), edgeLength, cubeColor, ambientValue, diffuseValue, reflectionValue, specularValue, shininessValue);
  }

  double intersect_t(Vector rayOrigin, Vector rayDirection) {
    double closestIntersection = -1.0;

    for (int faceIndex = 0; faceIndex < 6; faceIndex++) {
      double intersectionDistance = cubeFaces[faceIndex].intersect_t(rayOrigin, rayDirection);
      if (intersectionDistance > -EPSILON) {
        if (closestIntersection < 0.0) {
          closestIntersection = intersectionDistance;
        } else {
          closestIntersection = std::min(closestIntersection, intersectionDistance);
        }
      }
    }
    
    return closestIntersection;
  }

  // implement this function properly
  Vector normal(Vector surfacePoint) {
    double minDistance = -1.0;
    Vector closestNormal;

    for (int faceIndex = 0; faceIndex < 6; faceIndex++) {
      Vector faceNormal = cubeFaces[faceIndex].normal(surfacePoint);
      double distanceToCenter = (surfacePoint - ((Square)cubeFaces[faceIndex]).centroid()).length();
      if (minDistance < 0.0 || distanceToCenter < minDistance) {
        minDistance = distanceToCenter;
        closestNormal = faceNormal;
      }
    }

    return closestNormal;
  }

  Color getColor() {
    return Object::getColor();
  }

  Color getColor(Vector surfacePoint) {
    return getColor();
  }

  void draw() {
    for (int faceIndex = 0; faceIndex < 6; faceIndex++) {
      cubeFaces[faceIndex].draw();
    }
  }
};
#endif