#include <bits/stdc++.h>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <cstdlib>
#include "helper.h"
#ifdef __APPLE__
#include <GLUT/glut.h> // Use GLUT framework on macOS
#else
#include <GL/glut.h> // Use standard GLUT location on Linux/Windows
#endif

double horizontal_rotation_angle;
point preserve_right;
unsigned int refreshMillis=10;
GLfloat baseRotateAngele = 0.0f;
GLfloat rotationAngle = 0.0f;
GLfloat rotationSpeed = 1.0;
GLfloat baseBoundary=0.05;
GLfloat poleBoundary = 0.01;
point base[8] = {
    {baseBoundary, 5 * baseBoundary, baseBoundary},
    {-baseBoundary, 5 * baseBoundary, baseBoundary},
    {-baseBoundary, -5 * baseBoundary, baseBoundary},
    {baseBoundary, -5 * baseBoundary, baseBoundary},
    {baseBoundary, 5 * baseBoundary, -baseBoundary},
    {-baseBoundary, 5 * baseBoundary, -baseBoundary},
    {-baseBoundary, -5 * baseBoundary, -baseBoundary},
    {baseBoundary, -5 * baseBoundary, -baseBoundary}
};
point pole[8] = {
    {10 * poleBoundary, poleBoundary, poleBoundary},
    {-10 * poleBoundary, poleBoundary, poleBoundary},
    {-10 * poleBoundary, -poleBoundary, poleBoundary},
    {10 * poleBoundary, -poleBoundary, poleBoundary},
    {10 * poleBoundary, poleBoundary, -poleBoundary},
    {-10 * poleBoundary, poleBoundary, -poleBoundary},
    {-10 * poleBoundary, -poleBoundary, -poleBoundary},
    {10 * poleBoundary, -poleBoundary, -poleBoundary}};
void initGL();
void display();
void reshapeListener(GLsizei width, GLsizei height);
void keyboardListener(unsigned char key, int x, int y);
void specialKeyListener(int key, int x, int y);
void drawBase(point p1, point p2,point p3,point p4,point p5, point p6, point p7, point p8);
void drawPole(point p1, point p2,point p3,point p4,point p5, point p6, point p7, point p8);
void drawBlade();
void drawWindMill();
void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, 1, 1, 1000);
    position_of_camera = point(1 / sqrt(3), 1 / sqrt(3), 1 / sqrt(3));

    // camera_up=point(3.0/sqrt(18.0) ,0 , -3.0/sqrt(18.0) ) ;
    // camera_right=point(-1.0/sqrt(6.0) , 2.0/sqrt(6.0) , -1.0/sqrt(6.0) );
    // camera_look=point(-1.0/sqrt(3.0) , -1.0/sqrt(3.0) , -1.0/sqrt(3.0));

    camera_up = point(-1 / sqrt(6), 2 / sqrt(6), -1 / sqrt(6)).normalize();
    camera_right = point(1 / sqrt(2), 0.0, -1 / sqrt(2)).normalize();
    camera_look = point(-1 / sqrt(3), -1 / sqrt(3), -1 / sqrt(3)).normalize();
    // camera_look = point(-40, -40, -40).normalize();

    horizontal_rotation_angle = 0.0;
    preserve_right = point(0, 1, 0);
}
void drawLine(point p1, point p2)
{
    glBegin(GL_LINES);
    glVertex3f(p1.x, p1.y, p1.z);
    glVertex3f(p2.x, p2.y, p2.z);
    glEnd();
}
void drawAxis()
{
    glBegin(GL_LINES);
    drawLine({-0.5f, 0.0f, 0.0f}, {0.5f, 0.0f, 0.0f}); // X-axis
    drawLine({0.0f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f}); // Y-axis
    drawLine({0.0f, 0.0f, -0.5f}, {0.0f, 0.0f, 0.5f}); // Z-axis
    glEnd();
}
void display()
{
    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the model-view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Position camera using the eye, center and up vectors
    gluLookAt(position_of_camera.getx(), position_of_camera.gety(), position_of_camera.getz(),
              position_of_camera.getx() + camera_look.getx(), position_of_camera.gety() + camera_look.gety(), position_of_camera.getz() + camera_look.getz(),
              camera_up.getx(), camera_up.gety(), camera_up.getz());
    // glPushMatrix();
    // drawBase();
    // glPopMatrix();
    glColor3f(1.0f,1.0f,1.0f);
    drawAxis();                  // Draw the axes

    glRotatef(baseRotateAngele, 0.0f, 1.0f, 0.0f);
    // glPushMatrix();
    drawBase(base[0], base[1], base[2], base[3],
                 base[4], base[5], base[6], base[7]);
    // glPopMatrix();
    // glPushMatrix();
    glTranslatef(0.0, 0.2, 0.0);

    drawPole(pole[0], pole[1], pole[2], pole[3],
             pole[4], pole[5], pole[6], pole[7]);
    // glPopMatrix();      
    // glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f); // Cyan
    glRotatef(rotationAngle, 1.0, 0.0, 0.0);
    glTranslatef(0.1,0,0);
    drawWindMill();
    // glPopMatrix();   
    // glFlush(); 
    glutSwapBuffers();


}
/**
 * Window reshape callback
 * Handles window resizing and maintains aspect ratio
 */

void reshapeListener(GLsizei width, GLsizei height)
{
    // Prevent division by zero
    if (height == 0)
        height = 1;

    // Calculate aspect ratio
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set viewport to cover entire window
    glViewport(0, 0, width, height);

    // Set up perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 45-degree field of view, aspect ratio, near and far clipping planes
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}
void keyboardListener(unsigned char key, int x, int y)
{
    position_of_camera.show();
    camera_up.show();
    camera_right.show();
    camera_look.show();

    switch (key)
    {
        case '+':
         rotationSpeed+=1;
        break;
        case '-':
        {
            if(rotationSpeed>1.0)
              rotationSpeed-=1;
            break;}
        case 'a':
            baseRotateAngele += 5.0f;
            if(baseRotateAngele > 360.0)
                baseRotateAngele -= 360.f;
            break;
        case 'd':
            baseRotateAngele -= 5.0f;
            if(baseRotateAngele < 360.0)
                baseRotateAngele += 360.f;
            break;
        default:
        break;
    }
    glutPostRedisplay(); // Request a screen refresh
        
}    

void idle()
{
    glutPostRedisplay();
}    
void drawCircle(point center, float radius, int segments = 20) {
    glPushMatrix();
    
    // Translate to the center point
    glTranslatef(center.x, center.y, center.z);
    
    // Draw a filled circle
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 1.0f); // White circles
    
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        
        // We need to decide which plane to draw the circle on
        // This draws it in the Y-Z plane (facing X direction)
        glVertex3f(0, x, y);
    }
    glEnd();
    
    // Draw circle outline
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 0.0f, 0.0f); // Black outline
    
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        
        // Same plane as above
        glVertex3f(0, x, y);
    }
    glEnd();
    
    glPopMatrix();
}
void drawBlade(GLfloat radius){
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, radius, 0.05);
    glVertex3f(0.0, radius, -0.05);
    glEnd();
}
void drawFace(point p1, point p2,point p3,point p4)
{
    glBegin(GL_QUADS);
    glVertex3f(p1.x, p1.y, p1.z);
    glVertex3f(p2.x, p2.y, p2.z);
    glVertex3f(p3.x, p3.y, p3.z);
    glVertex3f(p4.x, p4.y, p4.z);
    glEnd();
    // float circleRadius = 0.01; // Adjust size as needed
    // drawCircle(p2, circleRadius);
}
void drawWindMill()
{
    glPushMatrix();
    drawBlade(0.2);
    glPopMatrix();

    glPushMatrix();
    glRotatef(120,1,0,0);
    drawBlade(0.2);
    glPopMatrix();

    glPushMatrix();
    glRotatef(240,1,0,0);
    drawBlade(0.2);
    glPopMatrix();
}    
void drawPole(point p1, point p2,point p3,point p4,point p5, point p6, point p7, point p8) {
    glColor3f(0.0f, 0.1f, 1.0f); 
    drawFace(p1, p2,p3,p4);
    drawFace(p5, p6, p7, p8);
    
    glColor3f(0.0f, 1.0f, 1.0f); // Green
    drawFace(p1, p4, p8, p5);
    drawFace(p2, p3, p7, p6);
}
// The base will be 4 rectangular faces.
void drawBase(point p1, point p2,point p3,point p4,point p5, point p6, point p7, point p8)
{
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    drawFace(p1, p2,p3,p4);
    drawFace(p5, p6, p7, p8);
    
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    drawFace(p1, p4, p8, p5);
    drawFace(p2, p3, p7, p6);
}
void timerFunction(int value)
{
    rotationAngle += rotationSpeed;
    // Request a redisplay
    glutPostRedisplay();

    // Register the timer again
    glutTimerFunc(refreshMillis, timerFunction, 0);
}

int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);

    // Configure display mode and window
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("OpenGL 3D Drawing");

    // Register callback functions
    glutTimerFunc(refreshMillis, timerFunction, 0);
    glutDisplayFunc(display);
    glutReshapeFunc(reshapeListener);
    glutKeyboardFunc(keyboardListener);

    // Initialize OpenGL settings
    initGL();
    glutIdleFunc(idle);
    // Enter the GLUT event loop
    glutMainLoop();

    return 0;
}