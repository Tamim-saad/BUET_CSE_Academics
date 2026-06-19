# Ray Tracer with Texture Support

A 3D ray tracer with OpenGL visualization and texture mapping capabilities.

## How to Build and Run

### Prerequisites
- Linux system with OpenGL development libraries
- g++ compiler
- GLUT library

### Build
```bash
cd /home/pridesys/Desktop/extra/410/solutions
chmod +x run.sh
./run.sh
```

Or compile manually:
```bash
g++ -o main main.cpp -lGL -lGLU -lglut
./main
```

## Controls

### Camera Movement
| Key | Function |
|-----|----------|
| Arrow Up | Move forward |
| Arrow Down | Move backward |
| Arrow Left | Move left |
| Arrow Right | Move right |
| Page Up | Move up |
| Page Down | Move down |

### Camera Rotation
| Key | Function |
|-----|----------|
| 1 | Rotate/Look left |
| 2 | Rotate/Look right |
| 3 | Look up |
| 4 | Look down |
| 5 | Tilt clockwise |
| 6 | Tilt counterclockwise |

### Texture Controls
| Key | Function |
|-----|----------|
| Space or T | **Cycle through texture modes:** |
|  | 1. Checkerboard (no texture) |
|  | 2. Texture1 (texture1.png) |
|  | 3. Texture2 (texture2.png) |
| + or = | Increase texture frequency (smaller tiles) |
| - or _ | Decrease texture frequency (larger tiles) |
| R | Reset texture scale to default |

### Capture
| Key | Function |
|-----|----------|
| 0 | Capture ray-traced image (saves as out.bmp) |

## Texture Modes

### 1. Checkerboard Mode (Default)
- Black and white alternating pattern
- No external texture files needed

### 2. Texture1 Mode  
- Uses `../spec/texture1.png` (511×511 pixels)
- Colorful texture pattern

### 3. Texture2 Mode
- Uses `../spec/texture2.png` (1280×1280 pixels) 
- Different colorful texture pattern

## Files Structure

### Core Files
- `main.cpp` - Main application with OpenGL setup
- `camera.hpp` - Camera system and ray tracing engine
- `checkerboard.hpp` - Floor/texture rendering system
- `texture_loader.hpp` - PNG texture loading using stb_image

### Object Classes
- `object.hpp` - Base object class
- `sphere.hpp` - Sphere objects
- `cube.hpp` - Cube objects
- `triangle.hpp` - Triangle objects
- `pyramid.hpp` - Pyramid objects
- `quadric.hpp` - Quadric surface objects

### Utilities
- `vector.hpp` - 3D vector operations
- `utils.hpp` - Common utilities and color definitions
- `bitmap_image.hpp` - BMP image handling
- `parser.hpp` - Scene file parsing

### Configuration
- `scene.txt` - Scene description file
- `description.txt` - View and render settings

## Texture Settings

- **Default Scale**: 0.05 (texture repeats every 20 units)
- **Scale Range**: 0.001 to 1.0
- **Supported Formats**: PNG (via stb_image)
- **Texture Location**: `../spec/texture1.png` and `../spec/texture2.png`

## Ray Tracing Features

- Recursive reflection
- Phong lighting model
- Shadow casting
- Multiple object types
- Configurable recursion levels
- BMP image output

## Troubleshooting

### Texture Not Loading
- Ensure `texture1.png` and `texture2.png` exist in `../spec/` directory
- Check console output for loading errors

### Compilation Errors
- Install OpenGL development packages: `sudo apt-get install freeglut3-dev`
- Ensure g++ is installed: `sudo apt-get install g++`

### Black Screen
- Use camera movement keys to navigate
- Press '0' to capture ray-traced view
- Ensure scene files are properly configured
