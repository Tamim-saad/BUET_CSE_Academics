#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <cmath>
#include <vector>
#include "vector.hpp"
#include "object.hpp"
#include "bitmap_image.hpp"
#include "utils.hpp"

class Camera {
private:
  Vector position;
  Vector lookAtPos;
  Vector upDir;

  double nearZ;
  double farZ;
  double fovY;
  double aspectRatio;
  int pixelsY;
  int maxLevelOfRecursion;
  
  void setUpDir();
  Vector pixelToVect(int x, int y);
  Color trace(const std::vector<Object *> &objects, const std::vector<Light> &lights, const Vector p, const Vector d, const int levelOfRecursion);

public:
  Camera();
  Camera(Vector position, Vector lookAtPos, Vector upDir);
  Vector getPosition();
  Vector getLookAtPos();
  Vector getLookDir();
  Vector getUpDir();
  Vector getRightDir();
  double getNearZ();
  double getFarZ();
  double getAspectRatio();
  double getFovY();
  int getPixelsY();

  void setCamera(Vector position, Vector lookAtPos, Vector upDir);
  void setNearZ(double z);
  void setFarZ(double z);
  void setAspectRatio(double r);
  void setFovY(double y);
  void setPixelsY(int n);

  void moveForward(double distance);
  void moveBackward(double distance);
  void moveLeft(double distance);
  void moveRight(double distance);
  void moveUp(double distance);
  void moveDown(double distance);
  void lookLeft(double angle);
  void lookRight(double angle);
  void lookUp(double angle);
  void lookDown(double angle);
  void tiltRight(double angle);
  void tiltLeft(double angle);
  void capture(const std::vector<Object *> &objects, const std::vector<Light> &lights
    , int levelOfRecursion);

  friend std::ostream &operator<<(std::ostream &out, const Camera &c);
};

Camera::Camera() {
  this->position = Vector(0, 0, 0);
  this->lookAtPos = Vector(0, 0, 1);
  this->upDir = Vector(0, 1, 0);
  this->maxLevelOfRecursion = 0;
}

Camera::Camera(Vector position, Vector lookAtPos, Vector upDir) {
  setCamera(position, lookAtPos, upDir);
}

void Camera::setCamera(Vector position, Vector lookAtPos, Vector upDir) {
  this->position = position;
  this->lookAtPos = lookAtPos;

  Vector lookDir = (lookAtPos - position);
  Vector rightDir = lookDir.cross(upDir);
  this->upDir = rightDir.cross(lookDir).normalize();
}

void Camera::setUpDir() {
  this->upDir = getRightDir().cross(getLookDir()).normalize();
}

Vector Camera::getPosition() {
  return this->position;
}

Vector Camera::getLookAtPos() {
  return this->lookAtPos;
}

Vector Camera::getLookDir() {
  return (this->lookAtPos - this->position).normalize();
}

Vector Camera::getUpDir() {
  return this->upDir;
}

Vector Camera::getRightDir() {
  return getLookDir().cross(this->upDir).normalize();
}

double Camera::getNearZ() {
  return this->nearZ;
}

double Camera::getFarZ() {
  return this->farZ;
}

double Camera::getFovY() {
  return this->fovY;
}

double Camera::getAspectRatio() {
  return this->aspectRatio;
}

int Camera::getPixelsY() {
  return this->pixelsY;
}

void Camera::setNearZ(double z) { 
  this->nearZ = z;
}

void Camera::setFarZ(double z) { 
  this->farZ = z; 
}

void Camera::setFovY(double y) { 
  this->fovY = y; 
}

void Camera::setAspectRatio(double r) { 
  this->aspectRatio = r; 
}

void Camera::setPixelsY(int n) {
  this->pixelsY = n;
}

void Camera::moveForward(double distance) {
  Vector lookAtDir = getLookDir();
  this->position = this->position + lookAtDir * distance;
  this->lookAtPos = this->lookAtPos + lookAtDir * distance;
}

void Camera::moveBackward(double distance) {
  Vector lookAtDir = getLookDir();
  this->position = this->position - lookAtDir * distance;
  this->lookAtPos = this->lookAtPos - lookAtDir * distance;
}

void Camera::moveLeft(double distance) {
  Vector rightDir = getRightDir();
  this->position = this->position - rightDir * distance;
  this->lookAtPos = this->lookAtPos - rightDir * distance;
}

void Camera::moveRight(double distance) {
  Vector rightDir = getRightDir();
  this->position = this->position + rightDir * distance;
  this->lookAtPos = this->lookAtPos + rightDir * distance;
}

void Camera::moveUp(double distance) {
  Vector upDir = getUpDir();
  this->position = this->position + upDir * distance;
  this->lookAtPos = this->lookAtPos + upDir * distance;
}

void Camera::moveDown(double distance) {
  Vector upDir = getUpDir();
  this->position = this->position - upDir * distance;
  this->lookAtPos = this->lookAtPos - upDir * distance;
}

void Camera::lookLeft(double angle) {
  Vector lookAtDir = getLookDir();
  Vector newLookAtDir = lookAtDir.rotate(this->upDir, angle);
  this->lookAtPos = this->position + newLookAtDir;
}

void Camera::lookRight(double angle) {
  lookLeft(-angle);
}

void Camera::lookUp(double angle) {
  Vector lookAtDir = getLookDir();
  Vector newLookAtDir = lookAtDir.rotate(getRightDir(), angle);
  this->lookAtPos = this->position + newLookAtDir;
  setUpDir();
}

void Camera::lookDown(double angle) {
  lookUp(-angle);
}

void Camera::tiltRight(double angle) {
  Vector upDir = getUpDir();
  Vector newUpDir = upDir.rotate(getLookDir(), angle);
  this->upDir = newUpDir;
}

void Camera::tiltLeft(double angle) {
  tiltRight(-angle);
}

Vector Camera::pixelToVect(int pxX, int pxY) {
  double fovX = fovY * aspectRatio;
  double fovXInRadians = fovX * M_PI / 180;
  double fovYInRadians = fovY * M_PI / 180;
  Vector middleOfScreen = position + getLookDir() * nearZ;
  int pixelsX = pixelsY * aspectRatio;

  double screenWidthInWorld = 2 * nearZ * tan(fovXInRadians / 2);
  double screenHeightInWorld = 2 * nearZ * tan(fovYInRadians / 2);
  double xRatio = screenWidthInWorld / pixelsX;
  double yRatio = screenHeightInWorld / pixelsY;

  double x = (pxX - pixelsX / 2) * xRatio;
  double y = -(pxY - pixelsY / 2) * yRatio;

  Vector p = middleOfScreen + getRightDir() * x + getUpDir() * y;

  return p;
}

Color Camera::trace(const std::vector<Object*>& objects, const std::vector<Light>& lights,
    Vector p, Vector d, const int levelOfRecursion) {

  if (levelOfRecursion == 0) {
    return {0, 0, 0};
  }

  double min_t = -1;
  Object* closestObject = nullptr;
  for (int i = 0; i < objects.size(); i++) {
    double t = objects[i]->intersect_t(p, d);
    if (t != -1) {
      if (min_t == -1 || t < min_t) {
        min_t = t;
        closestObject = objects[i];
      }
    }
  }

  if (closestObject == nullptr) {
    return {0, 0, 0};
  }

  // check if t is beyond the far plane
  if (levelOfRecursion == maxLevelOfRecursion) {
    double cosTheta = d.dot(getLookDir());
    double dist = farZ / cosTheta;
    if (min_t > dist) {
      return {0, 0, 0};
    }
  }

  Vector intersectionPoint = p + d * min_t;
  Vector normal = closestObject->normal(intersectionPoint);

  if (normal.dot(d) > 0) {
    normal = -1 * normal;
  }
  
  double lambert = 0;
  double phong = 0;
  Color lightContribution = {0, 0, 0};
  
  Vector reflectedRay = d - 2 * (normal.dot(d)) * normal;
  reflectedRay = reflectedRay.normalize();

  for (Light s : lights) {
    Vector ps = s.position - intersectionPoint;
    Vector toSource = ps.normalize();

    // if the light source is on the opposite side of camera from the object, ignore it
    if (normal.dot(toSource) < EPSILON) continue;

    if (s.cutOffAngle != 360) {
      Vector lightDir = (s.lookAt - s.position).normalize();
      double angle = acos(lightDir.dot(toSource * -1)) * 180 / M_PI;
      if (angle > s.cutOffAngle) continue;
    }
    
    double distance = ps.length();

    // check if the light source is blocked by another object
    bool blocked = false;
    for (Object *o : objects) {
      double t = o->intersect_t(intersectionPoint, toSource);
      if (t > EPSILON && t < distance) {
        blocked = true;
        break;
      }
    }

    if (blocked) continue;

    double scalingFactor = exp(-distance * distance * s.fallOff);
    double lambertFactor = toSource.dot(normal) * scalingFactor;
    double phongFactor = pow(reflectedRay.dot(toSource), closestObject->getShininess()) * scalingFactor;
    
    // Add light color contribution
    Color lightColor = {s.color.r, s.color.g, s.color.b};
    lightContribution = lightContribution + lightColor * (lambertFactor + phongFactor);
    
    lambert += lambertFactor;
    phong += phongFactor;
  }
  
  Vector intersectionPointWithEpsilon = intersectionPoint + normal * EPSILON;
  Color reflectedColor = trace(objects, lights, intersectionPointWithEpsilon, reflectedRay, levelOfRecursion - 1);

  Color color = closestObject->getColor(intersectionPoint);
  Color diffuse = Color{color.r * lightContribution.r, color.g * lightContribution.g, color.b * lightContribution.b} * closestObject->getDiffuse();
  Color specular = lightContribution * closestObject->getSpecular();
  Color ambient = color * closestObject->getAmbient();
  Color reflected = reflectedColor * closestObject->getReflection();

  return ambient + diffuse + specular + reflected;
}

void Camera::capture(const std::vector<Object*> &objects, const std::vector<Light>& lights
    , int levelOfRecursion) {
  std::cout << "Capturing image...\n" << std::endl;
  
  double fovX = fovY * aspectRatio;
  int pixelsX = pixelsY * aspectRatio;

  bitmap_image image(pixelsX, pixelsY);
  image.set_all_channels(0, 0, 0);

  int totalPixels = pixelsX * pixelsY;

  for (int i = 0; i < pixelsY; i++) {
    for (int j = 0; j < pixelsX; j++) {
      Vector p = pixelToVect(j, i);

      Vector d = p - position;
      d = d.normalize();

      maxLevelOfRecursion = levelOfRecursion;
      Color color = trace(objects, lights, p, d, levelOfRecursion);
      image.set_pixel(j, i, color.r * 255, color.g * 255, color.b * 255);

      int completion = (i * pixelsX + j) * 100 / totalPixels;
      std::cout << "\033[F";
      std::cout << "Progress: " << completion << "%" << std::endl;
    }
  }

  std::cout << "\033[F";
  std::cout << "Progress: 100%" << std::endl;

  image.save_image("out.bmp");
  std::cout << "Image saved to out.bmp" << std::endl;
}

std::ostream &operator<<(std::ostream &out, const Camera &c) {
  out << "Camera: " << std::endl;
  out << " Position: " << c.position << std::endl;
  out << " LookAtPos: " << c.lookAtPos << std::endl;
  out << " UpDir: " << c.upDir << std::endl;
  out << " Near: " << c.nearZ << std::endl;
  out << " Far: " << c.farZ << std::endl;
  out << " FovY: " << c.fovY << std::endl;
  out << " Aspect Ratio: " << c.aspectRatio << std::endl;

  return out;
}

#endif // CAMERA_HPP