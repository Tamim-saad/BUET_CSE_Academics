#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include "camera.hpp"
#include "scene_parser.hpp"
#include "utils.hpp"

using namespace std;

int main() {
    cout << "Ray Tracer - Headless Image Capture" << endl;
    
    // Setup scene parser
    SceneParser sceneParser("scene.txt");
    sceneParser.parse();
    sceneParser.printDescription();
    
    // Setup camera with scene parameters
    Vector defaultEye(0, -145, 40);
    Vector defaultLookAtPos(0, 0, 40);
    Vector defaultUpDir(0, 0, 1);
    
    Camera camera(defaultEye, defaultLookAtPos, defaultUpDir);
    
    auto d = sceneParser.getViewDescription();
    camera.setNearZ(1.0);      // Set reasonable defaults
    camera.setFarZ(1000.0);
    camera.setAspectRatio(1.0); // Square image
    camera.setFovY(60.0);      // 60 degree field of view
    camera.setPixelsY(d.numPixelsY);
    
    cout << "\nStarting ray tracing..." << endl;
    
    // Capture the image
    camera.capture(sceneParser.getObjects(), sceneParser.getLights(), d.levelOfRecursion);
    
    cout << "Ray tracing completed!" << endl;
    
    return 0;
}
