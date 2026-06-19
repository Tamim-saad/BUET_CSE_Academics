#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "sphere.hpp"
#include "cube.hpp"
#include "pyramid.hpp"
#include "quadrilateral.hpp"
#include "triangle.hpp"
#include "quadric.hpp"
#include "checkerboard.hpp"
#include "utils.hpp"

struct description {
  double near, far;
  double fovY;
  double aspectRatio;
  int levelOfRecursion;
  int numPixelsX, numPixelsY;
  double checkerBoardCellWidth;
  double checkerBoardAmbient, checkerBoardDiffuse, checkerBoardReflection;
};

class DescriptionParser {
private:
  std::vector<Object*> objects;
  Floor* floor;
  description viewDescription;
  std::vector<Light> lights;
  std::string descriptionFileName;

  void pushFloor() {
    Floor* floor = new Floor(viewDescription.checkerBoardCellWidth, viewDescription.checkerBoardAmbient
      , viewDescription.checkerBoardDiffuse, viewDescription.checkerBoardReflection, 0);
    this->floor = floor;
    objects.push_back(floor);
  }
public:
  DescriptionParser() {
    descriptionFileName = "description.txt";
  }

  DescriptionParser(std::string filename) : DescriptionParser() {
    descriptionFileName = filename;
  }

  void printDescription() {
    std::cout << "Near: " << viewDescription.near << std::endl;
    std::cout << "Far: " << viewDescription.far << std::endl;
    std::cout << "FovY: " << viewDescription.fovY << std::endl;
    std::cout << "Aspect Ratio: " << viewDescription.aspectRatio << std::endl;
    std::cout << "Level of Recursion: " << viewDescription.levelOfRecursion << std::endl;
    std::cout << "Num Pixels X: " << viewDescription.numPixelsX << std::endl;
    std::cout << "Num Pixels Y: " << viewDescription.numPixelsY << std::endl;
    std::cout << "Floor Cell Width: " << viewDescription.checkerBoardCellWidth << std::endl;
    std::cout << "Floor Ambient: " << viewDescription.checkerBoardAmbient << std::endl;
    std::cout << "Floor Diffuse: " << viewDescription.checkerBoardDiffuse << std::endl;
    std::cout << "Floor Reflection: " << viewDescription.checkerBoardReflection << std::endl;
  }

  description parse() {
    std::cout << "Parsing description file..." << std::endl;
    std::ifstream descriptionFile(descriptionFileName);
    std::string line;
    int lineNum = 0;

    while (getline(descriptionFile, line)) {
      if (line.empty()) {
        continue;
      }

      lineNum++;
      std::stringstream ss(line);

      if (lineNum == 1) {
        ss >> viewDescription.near >> viewDescription.far >> viewDescription.fovY >> viewDescription.aspectRatio;
      } else if (lineNum == 2) {
        ss >> viewDescription.levelOfRecursion;
      } else if (lineNum == 3) {
        ss >> viewDescription.numPixelsX;
        viewDescription.numPixelsY = viewDescription.numPixelsX;
      } else if (lineNum == 4) {
        ss >> viewDescription.checkerBoardCellWidth;
      } else if (lineNum == 5) {
        ss >> viewDescription.checkerBoardAmbient >> viewDescription.checkerBoardDiffuse >> viewDescription.checkerBoardReflection;
      } else if (lineNum == 6) {
        int numObjects;
        ss >> numObjects;

        for (int i = 0; i < numObjects; i++) {
          std::string objectType;

          while(getline(descriptionFile, objectType) && objectType.empty());

          if (objectType == "pyramid") {
            Vector lowest;
            GLfloat width, height;
            Color color;
            double ambient, diffuse, reflection, specular, shininess;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> lowest.x >> lowest.y >> lowest.z;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> width >> height;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> color.r >> color.g >> color.b;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> ambient >> diffuse >> specular >> reflection;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> shininess;

            objects.push_back(new Pyramid(lowest, width, height, color, ambient, diffuse, reflection, specular, shininess));
          }

          else if (objectType == "sphere") {
            Vector center;
            GLfloat radius;
            Color color;
            double ambient, diffuse, reflection, specular, shininess;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> center.x >> center.y >> center.z;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> radius;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> color.r >> color.g >> color.b;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> ambient >> diffuse >> specular >> reflection;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> shininess;

            objects.push_back(new Sphere(center, radius, color, ambient, diffuse, reflection, specular, shininess));
          } else if (objectType == "cube") {
            Vector bottomLowerLeft;
            GLfloat length;
            Color color;
            double ambient, diffuse, reflection, specular, shininess;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> bottomLowerLeft.x >> bottomLowerLeft.y >> bottomLowerLeft.z;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> length;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> color.r >> color.g >> color.b;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> ambient >> diffuse >> specular >> reflection;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> shininess;

            objects.push_back(new Cube(bottomLowerLeft, length, color, ambient, diffuse, reflection, specular, shininess));
          } else if (objectType == "triangle") {
            Vector p1, p2, p3;
            Color color;
            double ambient, diffuse, reflection, specular, shininess;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> p1.x >> p1.y >> p1.z;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> p2.x >> p2.y >> p2.z;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> p3.x >> p3.y >> p3.z;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> color.r >> color.g >> color.b;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> ambient >> diffuse >> specular >> reflection;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> shininess;

            objects.push_back(new Triangle(p1, p2, p3, color, ambient, diffuse, reflection, specular, shininess));
          } else if (objectType == "general") {
            double A, B, C, D, E, F, G, H, I, J;
            Vector refPoint;
            double length, width, height;
            Color color;
            double ambient, diffuse, reflection, specular, shininess;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> refPoint.x >> refPoint.y >> refPoint.z >> length >> width >> height;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> color.r >> color.g >> color.b;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> ambient >> diffuse >> specular >> reflection;

            getline(descriptionFile, line);
            ss = std::stringstream(line);
            ss >> shininess;

            objects.push_back(new Quadric(A, B, C, D, E, F, G, H, I, J, refPoint, length, width, height, color, ambient, diffuse, reflection, specular, shininess));
          }
        }
      } else if (lineNum == 7) {
        int numNormalLights;
        ss >> numNormalLights;

        for (int i = 0; i < numNormalLights; i++) {
          Vector position;
          double fallOff;
          
          while (getline(descriptionFile, line) && line.empty());

          ss = std::stringstream(line);
          ss >> position.x >> position.y >> position.z;
          
          getline(descriptionFile, line);
          ss = std::stringstream(line);
          ss >> fallOff;

          Light light;
          light.position = position;
          light.fallOff = fallOff;
          light.cutOffAngle = 360;
          lights.push_back(light);
        }
      } else if (lineNum == 8) {
        int numSpotLights;
        ss >> numSpotLights;

        for (int i = 0; i < numSpotLights; i++) {
          Vector position, lookAt;
          double fallOff, cutOffAngle;

          while (getline(descriptionFile, line) && line.empty());

          ss = std::stringstream(line);
          ss >> position.x >> position.y >> position.z;
          
          getline(descriptionFile, line);
          ss = std::stringstream(line);
          ss >> fallOff;

          getline(descriptionFile, line);
          ss = std::stringstream(line);
          ss >> lookAt.x >> lookAt.y >> lookAt.z >> cutOffAngle;

          Light light;
          light.position = position;
          light.lookAt = lookAt;
          light.fallOff = fallOff;
          light.cutOffAngle = cutOffAngle;
          lights.push_back(light);

          std::cout << "Spot light: " << light.position << " " << fallOff <<
            " " << light.cutOffAngle << " " << light.lookAt << std::endl;
        }
      } else {
        break;
      }
    }

    descriptionFile.close();

    pushFloor();
    return viewDescription;
  }

  description getViewDescription() {
    return viewDescription;
  }

  std::vector<Object*> getObjects() {
    return objects;
  }

  Floor* getFloor() {
    return floor;
  }

  std::vector<Light> getLights() {
    return lights;
  }

  ~DescriptionParser() {
    for (int i = 0; i < objects.size(); i++) {
      delete objects[i];
    }
  }
};

#endif