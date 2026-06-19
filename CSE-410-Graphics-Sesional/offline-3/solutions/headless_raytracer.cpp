#include <iostream>
#include <vector>
#include "camera.hpp"
#include "scene_parser.hpp"
#include "utils.hpp"

int main() {
    // Parse the scene
    SceneParser sceneParser("scene.txt");
    auto viewDescription = sceneParser.parse();
    
    sceneParser.printDescription();
    
    // Setup camera with proper parameters
    Vector defaultEye(0, -145, 40);
    Vector defaultLookAtPos(0, 0, 40);
    Vector defaultUpDir(0, 0, 1);
    
    Camera camera(defaultEye, defaultLookAtPos, defaultUpDir);
    camera.setNearZ(viewDescription.near);
    camera.setFarZ(viewDescription.far);
    camera.setAspectRatio(viewDescription.aspectRatio);
    camera.setFovY(viewDescription.fovY);
    camera.setPixelsY(viewDescription.numPixelsY);
    
    std::cout << "\nStarting ray tracing..." << std::endl;
    std::cout << "This may take a while for 2000x2000 image..." << std::endl;
    
    // Capture the image
    camera.capture(sceneParser.getObjects(), sceneParser.getLights(), viewDescription.levelOfRecursion);
    
    std::cout << "Ray tracing complete!" << std::endl;
    
    return 0;
}
