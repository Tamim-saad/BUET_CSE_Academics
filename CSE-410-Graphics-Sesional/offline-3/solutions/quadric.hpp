#ifndef QUADRIC_HPP
#define QUADRIC_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"

class Quadric : public Object {
private:
  double A, B, C, D, E, F, G, H, I, J;
  Vector refPoint;
  double length, width, height;

public:
  Quadric(double A, double B, double C, double D, double E, double F, 
          double G, double H, double I, double J,
          Vector refPoint, double length, double width, double height,
          Color color, double ambient, double diffuse, double reflection, double specular, double shininess) 
    : Object(color, ambient, diffuse, reflection, specular, shininess) {
    this->A = A;
    this->B = B;
    this->C = C;
    this->D = D;
    this->E = E;
    this->F = F;
    this->G = G;
    this->H = H;
    this->I = I;
    this->J = J;
    this->refPoint = refPoint;
    this->length = length;
    this->width = width;
    this->height = height;
  }

  double intersect_t(const Vector p, const Vector d) override {
    // Quadric equation: Ax² + By² + Cz² + Dxy + Exz + Fyz + Gx + Hy + Iz + J = 0
    // Substituting ray equation: P = p + td
    
    double a = A * d.x * d.x + B * d.y * d.y + C * d.z * d.z + 
               D * d.x * d.y + E * d.x * d.z + F * d.y * d.z;
    
    double b = 2 * A * p.x * d.x + 2 * B * p.y * d.y + 2 * C * p.z * d.z + 
               D * (p.x * d.y + p.y * d.x) + E * (p.x * d.z + p.z * d.x) + 
               F * (p.y * d.z + p.z * d.y) + G * d.x + H * d.y + I * d.z;
    
    double c = A * p.x * p.x + B * p.y * p.y + C * p.z * p.z + 
               D * p.x * p.y + E * p.x * p.z + F * p.y * p.z + 
               G * p.x + H * p.y + I * p.z + J;

    double discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) {
      return -1;
    }
    
    double sqrtD = sqrt(discriminant);
    double t1 = (-b - sqrtD) / (2 * a);
    double t2 = (-b + sqrtD) / (2 * a);
    
    double t = -1;
    
    if (t1 > EPSILON) {
      t = t1;
    } else if (t2 > EPSILON) {
      t = t2;
    } else {
      return -1;
    }
    
    Vector intersection = p + d * t;
    
    // Check clipping constraints
    if (length > 0) {
      if (intersection.x < refPoint.x || intersection.x > refPoint.x + length) {
        if (t2 > EPSILON && t == t1) {
          t = t2;
          intersection = p + d * t;
          if (intersection.x < refPoint.x || intersection.x > refPoint.x + length) {
            return -1;
          }
        } else {
          return -1;
        }
      }
    }
    
    if (width > 0) {
      if (intersection.y < refPoint.y || intersection.y > refPoint.y + width) {
        if (t2 > EPSILON && t == t1) {
          t = t2;
          intersection = p + d * t;
          if (intersection.y < refPoint.y || intersection.y > refPoint.y + width) {
            return -1;
          }
        } else {
          return -1;
        }
      }
    }
    
    if (height > 0) {
      if (intersection.z < refPoint.z || intersection.z > refPoint.z + height) {
        if (t2 > EPSILON && t == t1) {
          t = t2;
          intersection = p + d * t;
          if (intersection.z < refPoint.z || intersection.z > refPoint.z + height) {
            return -1;
          }
        } else {
          return -1;
        }
      }
    }
    
    return t;
  }

  Vector normal(const Vector p) override {
    // Gradient of quadric equation gives normal
    double nx = 2 * A * p.x + D * p.y + E * p.z + G;
    double ny = 2 * B * p.y + D * p.x + F * p.z + H;
    double nz = 2 * C * p.z + E * p.x + F * p.y + I;
    
    Vector n(nx, ny, nz);
    return n.normalize();
  }

  void draw() override {
    glColor3f(getColor().r, getColor().g, getColor().b);
    // For visualization, we'll draw a simplified representation
    glPushMatrix();
    glTranslatef(refPoint.x, refPoint.y, refPoint.z);
    
    // Simple cube representation for general quadrics
    if (length > 0 && width > 0 && height > 0) {
      glScalef(length/20, width/20, height/20);
      glutSolidCube(20);
    } else {
      glutSolidSphere(10, 20, 20);
    }
    
    glPopMatrix();
  }
};

#endif // QUADRIC_HPP
