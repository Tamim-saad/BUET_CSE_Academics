#ifndef QUADRILATERAL_HPP
#define QUADRILATERAL_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"
#include "triangle.hpp"


class Quadrilateral : public Object {
private:
  Vector lowerLeft, lowerRight, upperLeft, upperRight;
public:
  Quadrilateral() {
    lowerLeft = {0, 0, 0};
    lowerRight = {0, 0, 0};
    upperLeft = {0, 0, 0};
    upperRight = {0, 0, 0};
  }
  
  Quadrilateral(Vector lowerLeft, Vector lowerRight, Vector upperRight, Vector upperLeft, Color color, double ambient, double diffuse, double reflection, double specular, double shininess)
      : Object(color, ambient, diffuse, reflection, specular, shininess) {
    this->lowerLeft = lowerLeft;
    this->lowerRight = lowerRight;
    this->upperLeft = upperLeft;
    this->upperRight = upperRight;
  }

  Vector normal(const Vector p) {
    Vector v1 = lowerRight - lowerLeft;
    Vector v2 = upperLeft - lowerLeft;

    return v1.cross(v2).normalize();
  }

  double intersect_t(const Vector p, const Vector d) {
    // Barycentric coordinates
    // generate two triangles and call intersect_t on both
    // return the minimum of the two

    Triangle t1 = Triangle(lowerLeft, lowerRight, upperLeft, getColor(), getAmbient(), getDiffuse(), getReflection(), getSpecular(), getShininess());
    Triangle t2 = Triangle(lowerRight, upperRight, upperLeft, getColor(), getAmbient(), getDiffuse(), getReflection(), getSpecular(), getShininess());
  
    double t1_t = t1.intersect_t(p, d);
    double t2_t = t2.intersect_t(p, d);

    if (t1_t == -1 && t2_t == -1) {
      return -1;
    } else if (t1_t == -1) {
      return t2_t;
    } else if (t2_t == -1) {
      return t1_t;
    } else {
      return std::min(t1_t, t2_t);
    }
  }

  Color getColor() {
    return Object::getColor();
  }

  Color getColor(Vector p) {
    return getColor();
  }

  Vector centroid() {
    return (lowerLeft + lowerRight + upperLeft + upperRight) / 4;
  }

  void draw() {
    glColor3f(getColor().r, getColor().g, getColor().b);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(lowerLeft.x, lowerLeft.y, lowerLeft.z);
    glVertex3f(lowerRight.x, lowerRight.y, lowerRight.z);
    glVertex3f(upperRight.x, upperRight.y, upperRight.z);
    glVertex3f(upperLeft.x, upperLeft.y, upperLeft.z);
    glEnd();
    glPopMatrix();
  }
};

#endif