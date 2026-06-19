#ifndef SCENE_PARSER_HPP
#define SCENE_PARSER_HPP

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

struct sceneDescription {
  double near, far;
  double fovY;
  double aspectRatio;
  int levelOfRecursion;
  int numPixelsX, numPixelsY;
  double checkerBoardCellWidth;
  double checkerBoardAmbient, checkerBoardDiffuse, checkerBoardReflection;
};

class SceneParser {
private:
  std::vector<Object*> objects;
  Floor* floor;
  sceneDescription viewDescription;
  std::vector<Light> lights;
  std::string sceneFileName;

  void pushFloor() {
    double cellWidth = 20.0; // Default tile width from spec
    Floor* floor = new Floor(cellWidth, 0.2, 0.5, 0.3, 0);
    this->floor = floor;
    objects.push_back(floor);
  }

public:
  SceneParser() {
    sceneFileName = "../spec/scene.txt";
  }

  SceneParser(std::string filename) : SceneParser() {
    sceneFileName = filename;
  }

  void printDescription() {
    std::cout << "Scene Description:" << std::endl;
    std::cout << " Level of Recursion: " << viewDescription.levelOfRecursion << std::endl;
    std::cout << " Pixels: " << viewDescription.numPixelsX << " x " << viewDescription.numPixelsY << std::endl;
    std::cout << " Number of Objects: " << objects.size() - 1 << std::endl; // -1 for checkerboard
    std::cout << " Number of Lights: " << lights.size() << std::endl;
  }

  sceneDescription parse() {
    std::ifstream sceneFile(sceneFileName);
    if (!sceneFile.is_open()) {
      std::cerr << "Unable to open scene file: " << sceneFileName << std::endl;
      return viewDescription;
    }

    std::string line;
    std::stringstream ss;
    
    // Line 1: Level of recursion
    getline(sceneFile, line);
    ss = std::stringstream(line);
    ss >> viewDescription.levelOfRecursion;
    
    // Line 2: Pixels dimension
    getline(sceneFile, line);
    ss = std::stringstream(line);
    ss >> viewDescription.numPixelsX;
    viewDescription.numPixelsY = viewDescription.numPixelsX;
    
    // Empty line
    getline(sceneFile, line);
    
    // Line 3: Number of objects
    getline(sceneFile, line);
    ss = std::stringstream(line);
    int numObjects;
    ss >> numObjects;
    
    // Parse objects
    for (int i = 0; i < numObjects; i++) {
      std::string objectType;
      
      // Skip empty lines and get the object type
      do {
        getline(sceneFile, objectType);
        // Trim whitespace
        objectType.erase(objectType.find_last_not_of(" \t\r\n") + 1);
      } while (objectType.empty() && !sceneFile.eof());
      
      std::cout << "Parsing object " << (i+1) << ": " << objectType << std::endl;
      
      if (objectType == "sphere") {
        Vector center;
        double radius;
        Color color;
        double ambient, diffuse, specular, reflection;
        double shininess;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> center.x >> center.y >> center.z;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> radius;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> color.r >> color.g >> color.b;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> ambient >> diffuse >> specular >> reflection;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> shininess;

        objects.push_back(new Sphere(center, radius, color, ambient, diffuse, reflection, specular, shininess));
      } 
      else if (objectType == "triangle") {
        Vector p1, p2, p3;
        Color color;
        double ambient, diffuse, specular, reflection;
        double shininess;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> p1.x >> p1.y >> p1.z;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> p2.x >> p2.y >> p2.z;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> p3.x >> p3.y >> p3.z;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> color.r >> color.g >> color.b;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> ambient >> diffuse >> specular >> reflection;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> shininess;

        objects.push_back(new Triangle(p1, p2, p3, color, ambient, diffuse, reflection, specular, shininess));
      }
      else if (objectType == "general") {
        double A, B, C, D, E, F, G, H, I, J;
        Vector refPoint;
        double length, width, height;
        Color color;
        double ambient, diffuse, specular, reflection;
        double shininess;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> refPoint.x >> refPoint.y >> refPoint.z >> length >> width >> height;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> color.r >> color.g >> color.b;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> ambient >> diffuse >> specular >> reflection;

        getline(sceneFile, line);
        ss = std::stringstream(line);
        ss >> shininess;

        objects.push_back(new Quadric(A, B, C, D, E, F, G, H, I, J, refPoint, length, width, height, color, ambient, diffuse, reflection, specular, shininess));
      }
    }
    
    // Skip empty lines before lights section
    do {
      getline(sceneFile, line);
    } while (line.empty() && !sceneFile.eof());
    
    // Parse point lights
    ss = std::stringstream(line);
    int numPointLights;
    ss >> numPointLights;
    
    for (int i = 0; i < numPointLights; i++) {
      Vector position;
      Color lightColor;
      
      getline(sceneFile, line);
      ss = std::stringstream(line);
      ss >> position.x >> position.y >> position.z;
      
      getline(sceneFile, line);
      ss = std::stringstream(line);
      ss >> lightColor.r >> lightColor.g >> lightColor.b;
      
      Light light;
      light.position = position;
      light.color = lightColor;
      light.fallOff = 0.000001; // Default falloff
      light.cutOffAngle = 360; // Point light
      lights.push_back(light);
    }
    
    // Skip empty lines before spotlights section
    do {
      getline(sceneFile, line);
    } while (line.empty() && !sceneFile.eof());
    
    // Parse spotlights
    ss = std::stringstream(line);
    int numSpotLights;
    ss >> numSpotLights;
    
    for (int i = 0; i < numSpotLights; i++) {
      Vector position, lookAt;
      Color lightColor;
      double cutOffAngle;
      
      getline(sceneFile, line);
      ss = std::stringstream(line);
      ss >> position.x >> position.y >> position.z;
      
      getline(sceneFile, line);
      ss = std::stringstream(line);
      ss >> lightColor.r >> lightColor.g >> lightColor.b;
      
      getline(sceneFile, line);
      ss = std::stringstream(line);
      ss >> lookAt.x >> lookAt.y >> lookAt.z;
      
      getline(sceneFile, line);
      ss = std::stringstream(line);
      ss >> cutOffAngle;
      
      Light light;
      light.position = position;
      light.lookAt = lookAt;
      light.color = lightColor;
      light.fallOff = 0.000001; // Default falloff
      light.cutOffAngle = cutOffAngle;
      lights.push_back(light);
    }
    
    sceneFile.close();
    
    // Set default camera parameters
    viewDescription.near = 1.0;
    viewDescription.far = 1000.0;
    viewDescription.fovY = 80.0;
    viewDescription.aspectRatio = 1.0;
    
    pushFloor();
    return viewDescription;
  }

  sceneDescription getViewDescription() {
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

  ~SceneParser() {
    for (int i = 0; i < objects.size(); i++) {
      delete objects[i];
    }
  }
};

#endif // SCENE_PARSER_HPP
