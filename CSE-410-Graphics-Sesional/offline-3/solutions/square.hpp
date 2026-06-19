#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"


/**
 * A simple square object that is parallel to either of the three axis.
 * According to the specification, all the square planes are parallel to either of the three axis.
 * No use of barycentric equation minimizes the complexity of the code. Using this in cube 
 * boils down the execution time from minutes to miliseconds (considering the scene does not contain
 * any pyramid). The code for a general quadrilateral is also provided in the solutions folder.
*/
class Square : public Object {
private:
  Vector maxCoordinatedPoint;
  Vector n;
  double length;
  double D; // plane equation: ax + by + cz + d = 0
            // where a, b, c are the components of the n
public:
  Square() {
    maxCoordinatedPoint = Vector(0, 0, 0);
  }

  /**
   * Takes in the lower left corner of the square, the length and the n.
   *
   * @param maxCoordinatedPoint The lower left corner of the square. (Must be the point with the largest x, y, or z values)
   * @param length The length of the square.
   * @param n The n of the square. (Must be parallel to either of the three axis)
   */
  Square(Vector maxCoordinatedPoint, Vector n, double length, Color color, double ambient, double diffuse, double reflection, double specular, double shininess)
      : Object(color, ambient, diffuse, reflection, specular, shininess) {
    // throw error if n is not parallel to any of the three axis
    double x = n.x;
    double y = n.y;
    double z = n.z;
    
    if (!(x == 0 && y ==0 && z != 0 ||
          x == 0 && y != 0 && z == 0 ||
          x != 0 && y == 0 && z == 0)) {
      std::cout << "Error: n must be parallel to either of the three axis" << std::endl;
      exit(1);
    }

    this->maxCoordinatedPoint = maxCoordinatedPoint;
    this->n = n.normalize();
    this->length = length;
    this->D = -n.dot(maxCoordinatedPoint);
  }

  Vector normal(const Vector p) {
    return n;
  }

  double intersect_t(const Vector p, const Vector d) {
    // check if ray is parallel to the plane
    if (d.dot(n) < EPSILON && d.dot(n) > -EPSILON) {
      return -1;
    }

    // calculate point of intersection from ray and plane
    // using plane equation

    // plane equation: Ax + By + Cz + D = 0

    // ray equation: p + td
    // where p is the origin of the ray
    // and d is the direction of the ray
    // and t is the distance from the origin to the point of intersection
    // substitute ray equation into plane equation
    // and solve for t
    double t = -1;
    Vector intersectionPoint;
    if (n.x) {
      // plane is parallel to yz plane
      t = -D / (n.x * d.x) - p.x / d.x;

      if (t < EPSILON) {
        return -1;
      }

      intersectionPoint = t * d + p;

      if (intersectionPoint.y < maxCoordinatedPoint.y + EPSILON && intersectionPoint.y > maxCoordinatedPoint.y - length - EPSILON &&
          intersectionPoint.z < maxCoordinatedPoint.z + EPSILON && intersectionPoint.z > maxCoordinatedPoint.z - length - EPSILON)
      {
        return t;
      }
    } else if (n.y) {
      // plane is parallel to xz plane
      t = -D / (n.y * d.y) - p.y / d.y;

      if (t < EPSILON) {
        return -1;
      }

      intersectionPoint = t * d + p;

      if (intersectionPoint.x < maxCoordinatedPoint.x + EPSILON && intersectionPoint.x > maxCoordinatedPoint.x - length - EPSILON &&
          intersectionPoint.z < maxCoordinatedPoint.z + EPSILON && intersectionPoint.z > maxCoordinatedPoint.z - length - EPSILON)
      {
        return t;
      }
    } else if (n.z) {
      // plane is parallel to xy plane
      t = -D / (n.z * d.z) - p.z / d.z;

      if (t < EPSILON) {
        return -1;
      }

      intersectionPoint = t * d + p;

      if (intersectionPoint.x < maxCoordinatedPoint.x + EPSILON && intersectionPoint.x > maxCoordinatedPoint.x - length - EPSILON &&
          intersectionPoint.y < maxCoordinatedPoint.y + EPSILON && intersectionPoint.y > maxCoordinatedPoint.y - length - EPSILON)
      {
        return t;
      }
    }

    return -1;
  }

  Vector centroid() {
    if (n.x) {
      return Vector(maxCoordinatedPoint.x, maxCoordinatedPoint.y - length / 2, maxCoordinatedPoint.z - length / 2);
    } else if (n.y) {
      return Vector(maxCoordinatedPoint.x - length / 2, maxCoordinatedPoint.y, maxCoordinatedPoint.z - length / 2);
    } else {
      return Vector(maxCoordinatedPoint.x - length / 2, maxCoordinatedPoint.y - length / 2, maxCoordinatedPoint.z);
    }
  }

  Color getColor()
  {
    return Object::getColor();
  }

  Color getColor(Vector p)
  {
    return getColor();
  }

  void draw()
  {
    glColor3f(getColor().r, getColor().g, getColor().b);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex3f(maxCoordinatedPoint.x, maxCoordinatedPoint.y, maxCoordinatedPoint.z);

    if (n.x) {
      glVertex3f(maxCoordinatedPoint.x, maxCoordinatedPoint.y - length, maxCoordinatedPoint.z);
      glVertex3f(maxCoordinatedPoint.x, maxCoordinatedPoint.y - length, maxCoordinatedPoint.z - length);
      glVertex3f(maxCoordinatedPoint.x, maxCoordinatedPoint.y, maxCoordinatedPoint.z - length);
    } else if (n.y) {
      glVertex3f(maxCoordinatedPoint.x - length, maxCoordinatedPoint.y, maxCoordinatedPoint.z);
      glVertex3f(maxCoordinatedPoint.x - length, maxCoordinatedPoint.y, maxCoordinatedPoint.z - length);
      glVertex3f(maxCoordinatedPoint.x, maxCoordinatedPoint.y, maxCoordinatedPoint.z - length);
    } else if (n.z) {
      glVertex3f(maxCoordinatedPoint.x - length, maxCoordinatedPoint.y, maxCoordinatedPoint.z);
      glVertex3f(maxCoordinatedPoint.x - length, maxCoordinatedPoint.y - length, maxCoordinatedPoint.z);
      glVertex3f(maxCoordinatedPoint.x, maxCoordinatedPoint.y - length, maxCoordinatedPoint.z);
    }

    glEnd();
    glPopMatrix();
  }
};

#endif