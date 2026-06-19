#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <cmath>
#include <GL/glut.h>
#include "object.hpp"
#include "matrix.hpp"


class Triangle : public Object {
private:
  Vector vertex1, vertex2, vertex3;
  
public:
  Triangle() : vertex1(0, 0, 0), vertex2(0, 0, 0), vertex3(0, 0, 0) {}
  
  Triangle(Vector firstVertex, Vector secondVertex, Vector thirdVertex, Color triangleColor, double ambientCoeff, double diffuseCoeff, double reflectionCoeff, double specularCoeff, double shininessVal) 
    : Object(triangleColor, ambientCoeff, diffuseCoeff, reflectionCoeff, specularCoeff, shininessVal), 
      vertex1(firstVertex), vertex2(secondVertex), vertex3(thirdVertex) {

    // A = Matrix(3, 3);
    // A.set(0, 0, p1.x - p2.x);
    // A.set(0, 1, p1.x - p3.x);
    // A.set(1, 0, p1.y - p2.y);
    // A.set(1, 1, p1.y - p3.y);
    // A.set(2, 0, p1.z - p2.z);
    // A.set(2, 1, p1.z - p3.z);

    // betaMat = Matrix(3, 3);
    // betaMat.set(0, 1, p1.x - p3.x);
    // betaMat.set(1, 1, p1.y - p3.y);
    // betaMat.set(2, 1, p1.z - p3.z);

    // gammaMat = Matrix(3, 3);
    // gammaMat.set(0, 0, p1.x - p2.x);
    // gammaMat.set(1, 0, p1.y - p2.y);
    // gammaMat.set(2, 0, p1.z - p2.z);

    // tMat = Matrix(3, 3);
    // tMat.set(0, 0, p1.x - p2.x);
    // tMat.set(0, 1, p1.x - p3.x);
    // tMat.set(1, 0, p1.y - p2.y);
    // tMat.set(1, 1, p1.y - p3.y);
    // tMat.set(2, 0, p1.z - p2.z);
    // tMat.set(2, 1, p1.z - p3.z);
  }

  Vector normal(const Vector surfacePoint) {
    Vector edge1 = vertex2 - vertex1;
    Vector edge2 = vertex3 - vertex1;

    return edge1.cross(edge2).normalize();
  }

  Color getColor() {
    return Object::getColor();
  }

  Color getColor(Vector surfacePoint) {
    return getColor();
  }

  Vector getCentroid() {
    return (vertex1 + vertex2 + vertex3) / 3.0;
  }

/*
  double intersect_t(const Vector p, const Vector d) {
    Barycentric coordinates
    A.set(0, 2, d.x);
    A.set(1, 2, d.y);
    A.set(2, 2, d.z);

    double detA = A.determinant();

    betaMat.set(0, 0, p1.x - p.x);
    betaMat.set(0, 2, d.x);

    betaMat.set(1, 0, p1.y - p.y);
    betaMat.set(1, 2, d.y);

    betaMat.set(2, 0, p1.z - p.z);
    betaMat.set(2, 2, d.z);

    double beta = betaMat.determinant() / detA;

    if (beta < EPSILON) {
      return -1;
    }

    gammaMat.set(0, 1, p1.x - p.x);
    gammaMat.set(0, 2, d.x);
    gammaMat.set(1, 1, p1.y - p.y);
    gammaMat.set(1, 2, d.y);
    gammaMat.set(2, 1, p1.z - p.z);
    gammaMat.set(2, 2, d.z);

    double gamma = gammaMat.determinant() / detA;

    if (gamma < EPSILON || beta + gamma > 1 - EPSILON) {
      return -1;
    }

    tMat.set(0, 2, p1.x - p.x);
    tMat.set(1, 2, p1.y - p.y);
    tMat.set(2, 2, p1.z - p.z);

    double t = tMat.determinant() / detA;

    if (t > -EPSILON) {
      return t;
    } 

    return -1;
  }
*/

  double intersect_t(const Vector rayOrigin, const Vector rayDirection) {
    Vector edge1 = vertex2 - vertex1;
    Vector edge2 = vertex3 - vertex1;
    Vector crossProduct = rayDirection.cross(edge2);
    double determinant = edge1.dot(crossProduct);

    if (determinant > -EPSILON && determinant < EPSILON) {
      return -1.0;
    }

    double inverseDeterminant = 1.0 / determinant;
    Vector originToVertex = rayOrigin - vertex1;
    double barycentricU = inverseDeterminant * originToVertex.dot(crossProduct);

    if (barycentricU < EPSILON || barycentricU > 1.0 - EPSILON) {
      return -1.0;
    }
    
    Vector crossProductQ = originToVertex.cross(edge1);
    double barycentricV = inverseDeterminant * rayDirection.dot(crossProductQ);

    if (barycentricV < EPSILON || barycentricU + barycentricV > 1.0 - EPSILON) {
      return -1.0;
    }

    double intersectionT = inverseDeterminant * edge2.dot(crossProductQ);
    if (intersectionT > -EPSILON) {
      return intersectionT;
    }

    return -1.0;
  }

  void draw() {
    Color triangleColor = getColor();
    glColor3f(triangleColor.r, triangleColor.g, triangleColor.b);
    glPushMatrix();
    glBegin(GL_TRIANGLES);
    glVertex3f(vertex1.x, vertex1.y, vertex1.z);
    glVertex3f(vertex2.x, vertex2.y, vertex2.z);
    glVertex3f(vertex3.x, vertex3.y, vertex3.z);
    glEnd();
    glPopMatrix();
  }
};
#endif