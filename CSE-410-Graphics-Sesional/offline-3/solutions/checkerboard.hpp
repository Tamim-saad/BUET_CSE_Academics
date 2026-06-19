#ifndef FLOOR_HPP
#define FLOOR_HPP

#include <cmath>
#include <string>
#include <stdexcept>
#include <GL/glut.h>
#include "object.hpp"
#include "bitmap_image.hpp"
#include "texture_loader.hpp"

class Floor : public Object {
private:
  double width;
  double startX, startY;
  int n = 50;
  bitmap_image texture;
  TextureLoader pngTexture;
  double floorMinX, floorMaxX, floorMinY, floorMaxY;
  int textureMode = 0; // 0=checkerboard, 1=texture1, 2=texture2
  std::vector<std::string> textureFiles;
  double textureScale = 0.05; // Default scale - smaller = more repetition
  double defaultTextureScale = 0.05; // Store default for reset

  Color getCheckerColor(int x, int y) {
    if (((x / (int)width) + (y / (int)width)) % 2 == 0) {
      return {1, 1, 1};
    } else {
      return {0, 0, 0};
    }
  }

  Color getTextureColor(double x, double y) {
    // Create texture coordinates that properly scale and repeat
    // Use configurable scale for texture detail
    
    // Simple UV mapping with proper scaling
    double u = x * textureScale;
    double v = y * textureScale;
    
    // Use fmod to create seamless tiling
    u = fmod(u, 1.0);
    v = fmod(v, 1.0);
    
    // Ensure positive coordinates
    if (u < 0) u += 1.0;
    if (v < 0) v += 1.0;
    
    // Try PNG texture first (for PNG files), then fall back to BMP
    if (pngTexture.isLoaded()) {
      return pngTexture.sampleTexture(u, v);
    } else if (texture.width() > 0 && texture.height() > 0) {
      // Fallback to bitmap_image for BMP files
      int pixel_x = (int)(u * (texture.width() - 1));
      int pixel_y = (int)((1.0 - v) * (texture.height() - 1)); // Flip Y
      pixel_x = std::max(0, std::min((int)texture.width() - 1, pixel_x));
      pixel_y = std::max(0, std::min((int)texture.height() - 1, pixel_y));
      rgb_t pixel = texture.get_pixel(pixel_x, pixel_y);
      return {pixel.red / 255.0, pixel.green / 255.0, pixel.blue / 255.0};
    } else {
      // Fallback to checkerboard if no texture is loaded
      return getCheckerColor((int)x, (int)y);
    }
  }

public:
  Floor() {
    width = 0;
  }

  Floor(double width, double ambient, double diffuse, double reflection, double shininess)
    : Object({0, 0, 0}, ambient, diffuse, reflection, 0, shininess) {
    this->width = width;
    n = 50;
    startX = -n * width / 2;
    startY = n * width / 2;
    
    // Initialize texture files list - only texture1 and texture2
    textureFiles = {
      "../spec/texture1.png",
      "../spec/texture2.png"
    };
    
    // Try to load the first texture
    loadTexture(0);
    
    // Set floor bounds for texture mapping
    floorMinX = -n * width / 2;
    floorMaxX = n * width / 2;
    floorMinY = -n * width / 2;
    floorMaxY = n * width / 2;
  }

  double intersect_t(const Vector p, const Vector d) {
    if (fabs(d.z) < EPSILON) return -1;
    double t = -p.z / d.z;
    if (t < EPSILON) return -1;
    return t;
  }

  void toggleTexture() {
    textureMode = (textureMode + 1) % 3; // Cycle through 0, 1, 2
    
    if (textureMode == 0) {
      std::cout << "Switched to: Checkerboard (no texture)" << std::endl;
    } else if (textureMode == 1) {
      loadTexture(0); // Load texture1
      std::cout << "Switched to: Texture1 (texture1.png)" << std::endl;
    } else if (textureMode == 2) {
      loadTexture(1); // Load texture2  
      std::cout << "Switched to: Texture2 (texture2.png)" << std::endl;
    }
  }

  void resetTextureScale() {
    textureScale = defaultTextureScale;
    std::cout << "Texture scale reset to default: " << textureScale << std::endl;
  }

  void adjustTextureScale(double factor) {
    textureScale *= factor;
    textureScale = std::max(0.001, std::min(1.0, textureScale)); // Clamp between 0.001 and 1.0
    std::cout << "Texture scale adjusted to: " << textureScale << " (repeat every " << (1.0/textureScale) << " units)" << std::endl;
  }

  void increaseTextureScale() { adjustTextureScale(1.5); }  // Make texture smaller/more frequent
  void decreaseTextureScale() { adjustTextureScale(0.667); } // Make texture larger/less frequent

  bool hasTexture() {
    return textureMode > 0;
  }

  void loadTexture(int index) {
    if (index >= 0 && index < (int)textureFiles.size()) {
      std::string filename = textureFiles[index];
      
      // Clear previous textures first
      pngTexture = TextureLoader(); // Reset PNG texture
      
      if (pngTexture.load(filename)) {
        std::cout << "Loaded texture: " << filename << std::endl;
      } else {
        std::cout << "Failed to load texture: " << filename << std::endl;
      }
    }
  }

  bool getHasTexture() {
    return textureMode > 0;
  }

  Vector normal(const Vector p) {
    return Vector(0, 0, 1);
  }

  Color getColor() {
    return Object::getColor();
  }

  Color getColor(Vector p) {
    double x = p.x;
    double y = p.y;
    if (textureMode > 0) {
      return getTextureColor(x, y);
    } else {
      return getCheckerColor((int)x, (int)y);
    }
  }
  
  void draw() {}

  void draw(Vector position) {
    int posX = (int)position.x;
    int posY = (int)position.z;
    posX = posX - (posX % (int)width);
    posY = posY - (posY % (int)width);
    startX = posX - n * (int)width / 2;
    startY = posY + n * (int)width / 2;
    for (int i = 0; i < n; i++) {
      int x = startX + i * (int)width;
      for (int j = 0; j < n; j++) {
        int y = startY - j * (int)width;
        Color color = (textureMode > 0) ? getTextureColor(x, y) : getCheckerColor(x, y);
        glColor3f(color.r, color.g, color.b);
        glBegin(GL_QUADS);
        glVertex3f(x, y, 0);
        glVertex3f(x + width, y, 0);
        glVertex3f(x + width, y + width, 0);
        glVertex3f(x, y + width, 0);
        glEnd();
      }
    }
  }
};

#endif // FLOOR_HPP