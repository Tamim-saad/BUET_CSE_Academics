#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#define STB_IMAGE_IMPLEMENTATION
#include "../spec/stb_image.h"
#include "utils.hpp"
#include <string>
#include <iostream>

class TextureLoader {
private:
    unsigned char* data;
    int width, height, channels;
    bool loaded;

public:
    TextureLoader() : data(nullptr), width(0), height(0), channels(0), loaded(false) {}
    
    TextureLoader(const std::string& filename) : data(nullptr), width(0), height(0), channels(0), loaded(false) {
        load(filename);
    }
    
    ~TextureLoader() {
        if (data) {
            stbi_image_free(data);
        }
    }
    
    bool load(const std::string& filename) {
        if (data) {
            stbi_image_free(data);
            data = nullptr;
        }
        
        data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
        loaded = (data != nullptr);
        
        if (loaded) {
            std::cout << "Loaded texture: " << filename << " (" << width << "x" << height << ", " << channels << " channels)" << std::endl;
        } else {
            std::cout << "Failed to load texture: " << filename << " - " << stbi_failure_reason() << std::endl;
        }
        
        return loaded;
    }
    
    Color sampleTexture(double u, double v) {
        if (!loaded || !data || width <= 0 || height <= 0) {
            return {0.5, 0.5, 0.5}; // Gray fallback
        }
        
        // Clamp u and v to [0,1]
        u = std::max(0.0, std::min(1.0, u));
        v = std::max(0.0, std::min(1.0, v));
        
        // Normalized -> pixel coords
        int pixel_x = (int)(u * (width - 1));
        int pixel_y = (int)((1.0 - v) * (height - 1)); // Flip Y
        
        // Safety clamp
        pixel_x = std::max(0, std::min(width - 1, pixel_x));
        pixel_y = std::max(0, std::min(height - 1, pixel_y));
        
        // Compute array index
        int index = (pixel_y * width + pixel_x) * channels;
        
        Color color;
        color.r = data[index] / 255.0;
        
        if (channels >= 2) {
            color.g = data[index + 1] / 255.0;
        } else {
            color.g = color.r; // Grayscale
        }
        
        if (channels >= 3) {
            color.b = data[index + 2] / 255.0;
        } else {
            color.b = color.r; // Grayscale
        }
        
        return color;
    }
    
    bool isLoaded() const { return loaded; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }
};

#endif // TEXTURE_LOADER_HPP
