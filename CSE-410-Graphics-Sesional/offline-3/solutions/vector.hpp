#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <iostream>

class Vector {
public:
  double x, y, z;

  Vector();
  Vector(double xVal, double yVal, double zVal);
  
  double length() const;
  Vector normalize() const;
  double dot(const Vector& otherVec) const;
  Vector cross(const Vector& otherVec) const;
  Vector rotate(const Vector& rotationAxis, double rotationAngle) const;
  
  Vector operator+(const Vector& otherVec) const;
  Vector operator*(const Vector& otherVec) const;
  Vector operator*(const double& scalar) const;
  Vector operator/(const Vector& otherVec) const;
  Vector operator/(const double& scalar) const;

  friend Vector operator-(const Vector &vec1, const Vector &vec2);
  friend Vector operator*(double scaleFactor, const Vector &vec);
  friend Vector operator/(double &scaleFactor, const Vector &vec);
  friend std::ostream &operator<<(std::ostream &outputStream, const Vector &vec);

};

Vector::Vector() : x(0.0), y(0.0), z(0.0) {}

Vector::Vector(double xVal, double yVal, double zVal) : x(xVal), y(yVal), z(zVal) {}

Vector Vector::operator+(const Vector& otherVec) const {
  return Vector(x + otherVec.x, y + otherVec.y, z + otherVec.z);
}

Vector operator-(const Vector& vec1, const Vector& vec2) {
  return Vector(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
}

Vector Vector::operator*(const Vector& otherVec) const {
  return Vector(x * otherVec.x, y * otherVec.y, z * otherVec.z);
}

Vector Vector::operator/(const Vector& otherVec) const {
  return Vector(x / otherVec.x, y / otherVec.y, z / otherVec.z);
}

Vector Vector::operator*(const double& scalar) const {
  return Vector(x * scalar, y * scalar, z * scalar);
}

Vector Vector::operator/(const double& scalar) const {
  return Vector(x / scalar, y / scalar, z / scalar);
}

Vector operator*(double scaleFactor, const Vector &vec) {
  return Vector(scaleFactor * vec.x, scaleFactor * vec.y, scaleFactor * vec.z);
}

Vector operator/(double &scaleFactor, const Vector &vec) {
  return Vector(scaleFactor / vec.x, scaleFactor / vec.y, scaleFactor / vec.z);
}

std::ostream &operator<<(std::ostream &outputStream, const Vector &vec) {
  outputStream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
  return outputStream;
}

double Vector::dot(const Vector& otherVec) const {
  return x * otherVec.x + y * otherVec.y + z * otherVec.z;
}

Vector Vector::cross(const Vector& otherVec) const {
  return Vector(
    y * otherVec.z - z * otherVec.y,
    z * otherVec.x - x * otherVec.z,
    x * otherVec.y - y * otherVec.x
  );
}

Vector Vector::rotate(const Vector& rotationAxis, double rotationAngle) const {
  const Vector normalizedAxis = rotationAxis.normalize();
  const double angleInRadians = rotationAngle * M_PI / 180.0;
  const double cosTheta = cos(angleInRadians);
  const double sinTheta = sin(angleInRadians);
  const double dotProd = normalizedAxis.dot(*this);
  
  return cosTheta * (*this) + 
         sinTheta * normalizedAxis.cross(*this) + 
         (1.0 - cosTheta) * dotProd * normalizedAxis;
}

double Vector::length() const {
  return sqrt(x * x + y * y + z * z);
}

Vector Vector::normalize() const {
  const double len = length();
  return Vector(x / len, y / len, z / len);
}

#endif