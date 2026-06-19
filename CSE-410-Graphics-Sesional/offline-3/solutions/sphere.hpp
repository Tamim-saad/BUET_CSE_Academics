#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"


class Sphere : public Object {
private:
  Vector sphereCenter;
  double sphereRadius;

public:
  Sphere(Vector centerPos, double radiusVal, Color sphereColor, double ambientCoeff, double diffuseCoeff, double reflectionCoeff, double specularCoeff, double shininessVal) 
    : Object(sphereColor, ambientCoeff, diffuseCoeff, reflectionCoeff, specularCoeff, shininessVal), 
      sphereCenter(centerPos), sphereRadius(radiusVal) {}

  double intersect_t(const Vector rayOrigin, const Vector rayDirection) {
    const Vector originToCenter = rayOrigin - sphereCenter;
    const double projectionLength = -originToCenter.dot(rayDirection);

    if (projectionLength < EPSILON) {
      return -1.0;
    }

    const double perpDistanceSquared = originToCenter.dot(originToCenter) - projectionLength * projectionLength;

    if (perpDistanceSquared > sphereRadius * sphereRadius) {
      return -1.0;
    }

    const double halfChordLength = sqrt(sphereRadius * sphereRadius - perpDistanceSquared);

    if (projectionLength - halfChordLength > -EPSILON) {
      return projectionLength - halfChordLength;
    } else if (projectionLength + halfChordLength > -EPSILON) {
      return projectionLength + halfChordLength;
    } else {
      return -1.0;
    }
    
    // vector calculation
    // Vector distFromCenter = p - center;
    // double b = distFromCenter.dot(d);
    // double c = distFromCenter.dot(distFromCenter) - radius * radius;

    // double delta = b * b - c;

    // if (delta < 0) {
    //   return -1;
    // }

    // double t1 = -b - sqrt(delta);
    // double t2 = -b + sqrt(delta);

    // if (t1 < EPSILON && t2 < EPSILON) {
    //   return -1;
    // }

    // if (t1 < EPSILON) {
    //   return t2;
    // }

    // if (t2 < EPSILON) {
    //   return t1;
    // }

    // return t1 < t2 ? t1 : t2;
  }

  Vector normal(const Vector surfacePoint) {
    return (surfacePoint - sphereCenter).normalize();
  }
  
  Color getColor() {
    return Object::getColor();
  }
  
  Color getColor(Vector surfacePoint) {
    return getColor();
  }
  
  void draw() {
    Color sphereColor = getColor();
    glColor3f(sphereColor.r, sphereColor.g, sphereColor.b);
    glPushMatrix();
    glTranslatef(sphereCenter.x, sphereCenter.y, sphereCenter.z);
    glutSolidSphere(sphereRadius, 100, 100);
    glPopMatrix();
  }
};

#endif