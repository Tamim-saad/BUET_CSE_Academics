// Modular OpenGL Windmill Demo

#include <bits/stdc++.h>            // Includes most standard C++ libraries
#include <stdio.h>                  // Standard I/O functions
#include <stdlib.h>                 // Standard library functions
#include <math.h>                   // Math functions
#include <ctime>                    // Time functions
#include <cstdlib>                  // C Standard Library
#ifdef __linux__                    // If compiling on Linux
    #include <GL/glut.h>            // Include GLUT for OpenGL
#elif 
    #include <windows.h>            // Windows-specific header
    #include <glut.h>               // GLUT for Windows
#endif
using namespace std;                // Use standard namespace

// --- Constants and Macros ---
#define pi (2*acos(0.0))            // Define pi using arccos

// --- 3D Point Class ---
class Point {
public:
    double x, y, z;                 // 3D coordinates
    Point(double a = 0, double b = 0, double c = 0) : x(a), y(b), z(c) {} // Constructor
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y, z + p.z); } // Vector addition
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y, z - p.z); } // Vector subtraction
    Point operator*(double n) const { return Point(x * n, y * n, z * n); }             // Scalar multiplication
    Point cross(const Point& p) const { // Cross product
        return Point(
            y * p.z - z * p.y,
            z * p.x - x * p.z,
            x * p.y - y * p.x
        );
    }
    Point normalize() const {       // Normalize vector
        double length = sqrt(x * x + y * y + z * z);
        if (length == 0) return Point(0, 0, 0);
        return Point(x / length, y / length, z / length);
    }
    void show(const string& str = "KEYBOARD") const { // Print point for debugging
        cout << str << " " << x << "," << y << "," << z << "---" << endl;
    }
};

// --- Camera Class ---
class Camera {
public:
    Point position, up, look, right;        // Camera vectors
    double horizontal_rotation_angle;       // Horizontal rotation angle
    Point preserve_right;                   // Preserved right vector

    Camera() : horizontal_rotation_angle(0.0), preserve_right(0, 1, 0) {} // Constructor

    void initialize() {                     // Initialize camera position and orientation
        position = Point(1 / sqrt(3), 1 / sqrt(3), 1 / sqrt(3));
        up = Point(-1 / sqrt(6), 2 / sqrt(6), -1 / sqrt(6)).normalize();
        right = Point(1 / sqrt(2), 0.0, -1 / sqrt(2)).normalize();
        look = Point(-1 / sqrt(3), -1 / sqrt(3), -1 / sqrt(3)).normalize();
    }

    void show() const {                     // Print camera vectors for debugging
        position.show("Camera Pos");
        up.show("Camera Up");
        right.show("Camera Right");
        look.show("Camera Look");
    }
};

// --- Windmill Model Class ---
class WindmillModel {
public:
    GLfloat baseBoundary, poleBoundary;     // Size of base and pole
    Point base[8], pole[8];                 // Vertices for base and pole

    WindmillModel(GLfloat baseB = 0.05, GLfloat poleB = 0.01)
        : baseBoundary(baseB), poleBoundary(poleB) {
        setBase();                          // Initialize base vertices
        setPole();                          // Initialize pole vertices
    }

    void setBase() {                        // Set base vertices
        base[0] = {baseBoundary, 5 * baseBoundary, baseBoundary};
        base[1] = {-baseBoundary, 5 * baseBoundary, baseBoundary};
        base[2] = {-baseBoundary, -5 * baseBoundary, baseBoundary};
        base[3] = {baseBoundary, -5 * baseBoundary, baseBoundary};
        base[4] = {baseBoundary, 5 * baseBoundary, -baseBoundary};
        base[5] = {-baseBoundary, 5 * baseBoundary, -baseBoundary};
        base[6] = {-baseBoundary, -5 * baseBoundary, -baseBoundary};
        base[7] = {baseBoundary, -5 * baseBoundary, -baseBoundary};
    }

    void setPole() {                        // Set pole vertices
        pole[0] = {10 * poleBoundary, poleBoundary, poleBoundary};
        pole[1] = {-10 * poleBoundary, poleBoundary, poleBoundary};
        pole[2] = {-10 * poleBoundary, -poleBoundary, poleBoundary};
        pole[3] = {10 * poleBoundary, -poleBoundary, poleBoundary};
        pole[4] = {10 * poleBoundary, poleBoundary, -poleBoundary};
        pole[5] = {-10 * poleBoundary, poleBoundary, -poleBoundary};
        pole[6] = {-10 * poleBoundary, -poleBoundary, -poleBoundary};
        pole[7] = {10 * poleBoundary, -poleBoundary, -poleBoundary};
    }
};

// --- Drawing Utility Namespace ---
namespace DrawUtil {
    void line(const Point& p1, const Point& p2) {
        glBegin(GL_LINES);                  // Start drawing lines
        glVertex3f(p1.x, p1.y, p1.z);       // Line start
        glVertex3f(p2.x, p2.y, p2.z);       // Line end
        glEnd();                            // End drawing
    }

    void axis() {                           // Draw X, Y, Z axes
        glBegin(GL_LINES);
        line({-0.5f, 0.0f, 0.0f}, {0.5f, 0.0f, 0.0f}); // X-axis
        line({0.0f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f}); // Y-axis
        line({0.0f, 0.0f, -0.5f}, {0.0f, 0.0f, 0.5f}); // Z-axis
        glEnd();
    }

    void face(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
        glBegin(GL_QUADS);                  // Start drawing quad
        glVertex3f(p1.x, p1.y, p1.z);       // First vertex
        glVertex3f(p2.x, p2.y, p2.z);       // Second vertex
        glVertex3f(p3.x, p3.y, p3.z);       // Third vertex
        glVertex3f(p4.x, p4.y, p4.z);       // Fourth vertex
        glEnd();                            // End drawing
    }

    void circle(const Point& center, float radius, int segments = 20) {
        glPushMatrix();                     // Save current matrix
        glTranslatef(center.x, center.y, center.z); // Move to center
        glBegin(GL_POLYGON);                // Start drawing polygon
        glColor3f(1.0f, 1.0f, 1.0f);        // Set color to white
        for (int i = 0; i < segments; i++) {
            float theta = 2.0f * M_PI * float(i) / float(segments); // Angle for segment
            float x = radius * cosf(theta); // X coordinate
            float y = radius * sinf(theta); // Y coordinate
            glVertex3f(0, x, y);            // Draw vertex in Y-Z plane
        }
        glEnd();
        // Outline
        glBegin(GL_LINE_LOOP);              // Start drawing outline
        glColor3f(0.0f, 0.0f, 0.0f);        // Set color to black
        for (int i = 0; i < segments; i++) {
            float theta = 2.0f * M_PI * float(i) / float(segments);
            float x = radius * cosf(theta);
            float y = radius * sinf(theta);
            glVertex3f(0, x, y);
        }
        glEnd();
        glPopMatrix();                      // Restore matrix
    }

    void blade(GLfloat radius) {            // Draw a single windmill blade
        glBegin(GL_TRIANGLES);              // Start drawing triangle
        glVertex3f(0.0, 0.0, 0.0);         // Center point
        glVertex3f(0.0, radius, 0.05);     // Tip 1
        glVertex3f(0.0, radius, -0.05);    // Tip 2
        glEnd();
    }

    void windmill() {                       // Draw all windmill blades
        glPushMatrix();
        blade(0.2);                        // First blade
        glPopMatrix();
        glPushMatrix();
        glRotatef(120, 1, 0, 0);           // Rotate for second blade
        blade(0.2);
        glPopMatrix();
        glPushMatrix();
        glRotatef(240, 1, 0, 0);           // Rotate for third blade
        blade(0.2);
        glPopMatrix();
    }

    void pole(const Point* p) {             // Draw windmill pole
        glColor3f(0.0f, 0.1f, 1.0f);       // Set color
        face(p[0], p[1], p[2], p[3]);      // Draw faces
        face(p[4], p[5], p[6], p[7]);
        glColor3f(0.0f, 1.0f, 1.0f);       // Set color to cyan
        face(p[0], p[3], p[7], p[4]);
        face(p[1], p[2], p[6], p[5]);
    }

    void base(const Point* p) {             // Draw windmill base
        glColor3f(0.0f, 0.0f, 1.0f);       // Set color to blue
        face(p[0], p[1], p[2], p[3]);
        face(p[4], p[5], p[6], p[7]);
        glColor3f(0.0f, 1.0f, 0.0f);       // Set color to green
        face(p[0], p[3], p[7], p[4]);
        face(p[1], p[2], p[6], p[5]);
    }
}

// --- Global Variables ---
Camera camera;                              // Camera object
WindmillModel windmill;                     // Windmill model object
unsigned int refreshMillis = 10;            // Timer refresh interval (ms)
GLfloat baseRotateAngle = 0.0f;             // Rotation angle for base
GLfloat rotationAngle = 0.0f;               // Rotation angle for blades
GLfloat rotationSpeed = 1.0;                // Speed of blade rotation

// --- OpenGL Initialization ---
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Set background color to black
    glEnable(GL_DEPTH_TEST);                // Enable depth testing
    glMatrixMode(GL_PROJECTION);            // Switch to projection matrix
    glLoadIdentity();                       // Reset projection matrix
    gluPerspective(70, 1, 1, 1000);         // Set perspective projection
    camera.initialize();                    // Initialize camera
}

// --- GLUT Display and Event Functions ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);             // Switch to modelview matrix
    glLoadIdentity();                       // Reset modelview matrix
    gluLookAt(camera.position.x, camera.position.y, camera.position.z, // Camera position
              camera.position.x + camera.look.x, camera.position.y + camera.look.y, camera.position.z + camera.look.z, // Look at point
              camera.up.x, camera.up.y, camera.up.z); // Up vector
    glColor3f(1.0f, 1.0f, 1.0f);           // Set color to white
    DrawUtil::axis();                       // Draw axes
    glRotatef(baseRotateAngle, 0.0f, 1.0f, 0.0f); // Rotate base around Y axis
    DrawUtil::base(windmill.base);          // Draw windmill base
    glTranslatef(0.0, 0.2, 0.0);            // Move up for pole
    DrawUtil::pole(windmill.pole);          // Draw windmill pole
    glColor3f(1.0f, 1.0f, 0.0f);            // Set color to yellow
    glRotatef(rotationAngle, 1.0, 0.0, 0.0); // Rotate blades around X axis
    glTranslatef(0.1, 0, 0);                // Move to blade center
    DrawUtil::windmill();                   // Draw windmill blades
    glutSwapBuffers();                      // Swap front and back buffers
}

void reshapeListener(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;            // Prevent division by zero
    GLfloat aspect = (GLfloat)width / (GLfloat)height; // Calculate aspect ratio
    glViewport(0, 0, width, height);        // Set viewport to window size
    glMatrixMode(GL_PROJECTION);            // Switch to projection matrix
    glLoadIdentity();                       // Reset projection matrix
    gluPerspective(45.0f, aspect, 0.1f, 100.0f); // Set perspective projection
}

void keyboardListener(unsigned char key, int x, int y) {
    camera.show();                          // Print camera info (debug)
    switch (key) {
        case '+':                           // Increase blade rotation speed
            rotationSpeed += 1;
            break;
        case '-':                           // Decrease blade rotation speed
            if (rotationSpeed > 1.0)
                rotationSpeed -= 1;
            break;
        case 'a':                           // Rotate base left
            baseRotateAngle += 5.0f;
            if (baseRotateAngle > 360.0)
                baseRotateAngle -= 360.f;
            break;
        case 'd':                           // Rotate base right
            baseRotateAngle -= 5.0f;
            if (baseRotateAngle < 0.0)
                baseRotateAngle += 360.f;
            break;
        default:                            // Ignore other keys
            break;
    }
    glutPostRedisplay();                    // Request redraw
}

void idle() {
    glutPostRedisplay();                    // Continuously redraw scene
}

void timerFunction(int value) {
    rotationAngle += rotationSpeed;         // Update blade rotation angle
    glutPostRedisplay();                    // Request redraw
    glutTimerFunc(refreshMillis, timerFunction, 0); // Set next timer event
}

// --- Main Function ---
int main(int argc, char **argv) {
    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Set display mode
    glutInitWindowSize(1000, 1000);         // Set window size
    glutInitWindowPosition(50, 50);         // Set window position
    glutCreateWindow("OpenGL 3D Drawing");  // Create window with title
    glutTimerFunc(refreshMillis, timerFunction, 0); // Start timer
    glutDisplayFunc(display);               // Register display callback
    glutReshapeFunc(reshapeListener);       // Register reshape callback
    glutKeyboardFunc(keyboardListener);     // Register keyboard callback
    initGL();                              // Initialize OpenGL settings
    glutIdleFunc(idle);                    // Register idle callback
    glutMainLoop();                        // Enter GLUT event processing loop
    return 0;                              // Exit program
}