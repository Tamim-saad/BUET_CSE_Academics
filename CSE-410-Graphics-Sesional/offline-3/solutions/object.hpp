#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "vector.hpp"
#include "utils.hpp"

class Object {
private:
  Color materialColor;
  double ambientCoefficient, diffuseCoefficient, reflectionCoefficient, specularCoefficient;
  double shininessExponent;
public:
  Object() : materialColor({0, 0, 0}), ambientCoefficient(0.0), diffuseCoefficient(0.0), 
             reflectionCoefficient(0.0), specularCoefficient(0.0), shininessExponent(0.0) {}

  Object(Color objectColor, double ambientValue, double diffuseValue, double reflectionValue, double specularValue, double shininessValue) 
    : materialColor(objectColor), ambientCoefficient(ambientValue), diffuseCoefficient(diffuseValue), 
      reflectionCoefficient(reflectionValue), specularCoefficient(specularValue), shininessExponent(shininessValue) {}
  /**
   * @param p: origin of the ray
   * @param d: direction of the ray (normalized)
   * @return: distance from p to the intersection point
  */
  virtual double intersect_t(const Vector rayOrigin, const Vector rayDirection) = 0;
  virtual Vector normal(const Vector surfacePoint) = 0; 
  virtual void draw() = 0;

  virtual Color getColor() {
    return materialColor;
  }

  virtual Color getColor(Vector surfacePoint) {
    return materialColor;
  }

  virtual double getAmbient() {
    return ambientCoefficient;
  }

  virtual double getDiffuse() {
    return diffuseCoefficient;
  }

  virtual double getReflection() {
    return reflectionCoefficient;
  }

  virtual double getSpecular() {
    return specularCoefficient;
  }

  virtual double getShininess() {
    return shininessExponent;
  }
};

#endif // OBJECT_HPP