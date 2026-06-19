#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include "camera.hpp"
#include "parser.hpp"
#include "scene_parser.hpp"
#include "utils.hpp"

#define INIT_WINDOW_WIDTH 700
#define INIT_WINDOW_HEIGHT 700
#define INIT_WINDOW_X 100
#define INIT_WINDOW_Y 100

using namespace std;

// Configuration variables
DescriptionParser parser("description.txt");
SceneParser sceneParser("scene.txt");
bool useSceneParser = true;
const Vector defaultEye(0, -145, 40);
const Vector defaultLookAtPos(0, 0, 40);
const Vector defaultUpDir(0, 0, 1);
bool isShowingAxes = true;

// Global state
Camera camera;

const GLfloat cameraRotationAmount = 10.0f;
const GLfloat cameraMoveAmount = 5.0f;
GLfloat rotationAngle = 0.0f;

void initGL() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
}

void reshapeListener(GLsizei width, GLsizei height) {
  if (height == 0)
    height = 1;
  GLfloat aspect = (GLfloat)width / (GLfloat)height;

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(camera.getFovY(), camera.getAspectRatio(), camera.getNearZ(),
    camera.getFarZ());
}

void keyboardListener(unsigned char key, int x, int y) {
  switch (key) {
    case 'a':
      camera.lookLeft(cameraRotationAmount);
      break;
    case 'd':
      camera.lookRight(cameraRotationAmount);
      break;
    case 'w':
      camera.lookUp(cameraRotationAmount);
      break;
    case 's':
      camera.lookDown(cameraRotationAmount);
      break;
    case 'q':
      camera.tiltLeft(cameraRotationAmount);
      break;
    case 'e':
      camera.tiltRight(cameraRotationAmount);
      break;
    case '1':
      camera.lookLeft(cameraRotationAmount);
      break;
    case '2':
      camera.lookRight(cameraRotationAmount);
      break;
    case '3':
      camera.lookUp(cameraRotationAmount);
      break;
    case '4':
      camera.lookDown(cameraRotationAmount);
      break;
    case '5':
      camera.tiltLeft(cameraRotationAmount);
      break;
    case '6':
      camera.tiltRight(cameraRotationAmount);
      break;
    case '0':
      if (useSceneParser) {
        camera.capture(sceneParser.getObjects(), sceneParser.getLights(),
          sceneParser.getViewDescription().levelOfRecursion);
      } else {
        camera.capture(parser.getObjects(), parser.getLights(),
          parser.getViewDescription().levelOfRecursion);
      }
      break;   
    case ' ':
      if (useSceneParser) {
        sceneParser.getFloor()->toggleTexture();
      } else {
        parser.getFloor()->toggleTexture();
      }
      break; 
    case 't':
    case 'T':
      if (useSceneParser) {
        sceneParser.getFloor()->toggleTexture();
      } else {
        parser.getFloor()->toggleTexture(); 
      }
      break;
    case '+':
    case '=':
      if (useSceneParser) {
        sceneParser.getFloor()->increaseTextureScale();
      } else {
        parser.getFloor()->increaseTextureScale(); 
      }
      break;
    case '-':
    case '_':
      if (useSceneParser) {
        sceneParser.getFloor()->decreaseTextureScale();
      } else {
        parser.getFloor()->decreaseTextureScale(); 
      }
      break;
    case 'r':
    case 'R':
      if (useSceneParser) {
        sceneParser.getFloor()->resetTextureScale();
      } else {
        parser.getFloor()->resetTextureScale(); 
      }
      break;
    default:
      return;
  }
  glutPostRedisplay(); // Post a paint request to activate display()
}

void specialKeyListener(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_UP:
      camera.moveForward(cameraRotationAmount);
      break;
    case GLUT_KEY_DOWN:
      camera.moveBackward(cameraRotationAmount);
      break;
    case GLUT_KEY_RIGHT:
      camera.moveRight(cameraRotationAmount);
      break;
    case GLUT_KEY_LEFT:
      camera.moveLeft(cameraRotationAmount);
      break;
    case GLUT_KEY_PAGE_UP:
      camera.moveUp(cameraMoveAmount);
      break;
    case GLUT_KEY_PAGE_DOWN:
      camera.moveDown(cameraMoveAmount);
      break;
    case GLUT_KEY_HOME:
      camera.setCamera(defaultEye, defaultLookAtPos, defaultUpDir);
      break;
    default:
      return;
  }

  glutPostRedisplay(); // Post a paint request to activate display()
}

void mouseListener(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    isShowingAxes = !isShowingAxes;
  }
  glutPostRedisplay();
}

void drawAxes(GLfloat length = 1.0) {
  glPushMatrix();
  glLineWidth(3);
  glScaled(length, length, length);
  glBegin(GL_LINES);
  glColor3f(1, 0, 0); // Red

  // X axis
  glVertex3f(0, 0, 0);
  glVertex3f(1, 0, 0);

  glColor3f(0, 1, 0); // Green
  // Y axis
  glVertex3f(0, 0, 0);
  glVertex3f(0, 1, 0);

  glColor3f(0, 0, 1); // Blue
  // Z axis
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 1);
  glEnd();
  glPopMatrix();
}

void drawInfiniteCheckerBoard(GLfloat widthOfEachCell) {
  int numberOfCells = 1000;

  GLfloat x = -numberOfCells * widthOfEachCell / 2;
  GLfloat y = 0;
  GLfloat z = -numberOfCells * widthOfEachCell / 2;

  for (int i = 0; i < numberOfCells; i++) {
    for (int j = 0; j < numberOfCells; j++) {
      if ((i + j) % 2 == 0) {
        glColor3f(1, 1, 1);
      } else {
        glColor3f(0, 0, 0);
      }
      glBegin(GL_QUADS);
      glVertex3f(x, y, z);
      glVertex3f(x + widthOfEachCell, y, z);
      glVertex3f(x + widthOfEachCell, y, z + widthOfEachCell);
      glVertex3f(x, y, z + widthOfEachCell);
      glEnd();
      x += widthOfEachCell;
    }
    x = -numberOfCells * widthOfEachCell / 2;
    z += widthOfEachCell;
  }
}

void drawCone(Vector position, Vector direction, double cutOffAngle, GLfloat scale, int slices)
{
  double radius = tan(cutOffAngle * M_PI / 360);
  double height = 1;
  direction = direction.normalize();

  double tempX = radius,  tempY = 0;
  double currX, currY;

  Vector rotationAxis = Vector(0, 0, -1).cross(direction).normalize();
  double rotationAngle = acos(Vector(0, 0, -1).dot(direction)) * 180 / M_PI;

  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);
  glRotatef(rotationAngle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
  glScaled(scale, scale, scale);

  glBegin(GL_TRIANGLES);
  for (int i = 0; i < slices; i++) {
    double theta = i * 2 * M_PI / slices;
    currX = radius * cos(theta);
    currY = radius * sin(theta);
    
    double c = (2 + cos(theta)) / 3;
    glColor3f(c, c, c);
    glVertex3f(0, 0, height / 2);
    glVertex3f(tempX, tempY, -height / 2);
    glVertex3f(currX, currY, -height / 2);

    tempX = currX;
    tempY = currY;
  }
  glEnd();
  glPopMatrix();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(camera.getFovY(), camera.getAspectRatio(), camera.getNearZ(), camera.getFarZ());
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(
    camera.getPosition().x, camera.getPosition().y, camera.getPosition().z,
    camera.getLookAtPos().x, camera.getLookAtPos().y, camera.getLookAtPos().z,
    camera.getUpDir().x, camera.getUpDir().y, camera.getUpDir().z
  );

  glRotated(rotationAngle, 0, 1, 0);

  if (isShowingAxes)
    drawAxes(1000.0);
  
  std::vector<Object*> objects;
  std::vector<Light> lights;
  
  if (useSceneParser) {
    objects = sceneParser.getObjects();
    lights = sceneParser.getLights();
  } else {
    objects = parser.getObjects();
    lights = parser.getLights();
  }
  
  for (int i = 0; i < objects.size(); i++) {
    objects[i]->draw();
  }

  if (useSceneParser) {
    sceneParser.getFloor()->draw(camera.getPosition());
  } else {
    parser.getFloor()->draw(camera.getPosition());
  }

  glColor3f(0.2, 0.2, 0.2);
  for (Light light : lights) {
    glPushMatrix();
    if (light.cutOffAngle != 360) {
      Vector lookDir = (light.lookAt - light.position).normalize();
      glColor3f(0.45, 0.45, .5);
      drawCone(light.position, lookDir, light.cutOffAngle, 15, 100);

    } else {      
    glTranslatef(light.position.x, light.position.y, light.position.z);
      glutSolidSphere(3, 50, 50);
    }
    
    glPopMatrix();
  }
  
  glutSwapBuffers();
}



int main(int argc, char **argv) {
  camera = Camera(defaultEye, defaultLookAtPos, defaultUpDir);

  if (useSceneParser) {
    sceneParser.parse();
    sceneParser.printDescription();
    
    auto d = sceneParser.getViewDescription();
    camera.setNearZ(d.near);
    camera.setFarZ(d.far);
    camera.setAspectRatio(d.aspectRatio);
    camera.setFovY(d.fovY);
    camera.setPixelsY(d.numPixelsY);
    
    // Auto-capture image for testing
    std::cout << "\nAuto-capturing image..." << std::endl;
    camera.capture(sceneParser.getObjects(), sceneParser.getLights()
      , sceneParser.getViewDescription().levelOfRecursion);
    std::cout << "Image capture completed! Check 'out.bmp' file." << std::endl;
  } else {
    parser.parse();
    parser.printDescription();

    auto d = parser.getViewDescription();
    camera.setNearZ(d.near);
    camera.setFarZ(d.far);
    camera.setAspectRatio(d.aspectRatio);
    camera.setFovY(d.fovY);
    camera.setPixelsY(d.numPixelsY);
  }

  // Initialize glut
  glutInit(&argc, argv);
  glutInitWindowSize(
    INIT_WINDOW_WIDTH, 
    INIT_WINDOW_HEIGHT
  );
  glutInitWindowPosition(
    INIT_WINDOW_X, 
    INIT_WINDOW_Y
  );

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Ray Tracer");
  glutDisplayFunc(display);
  glutReshapeFunc(reshapeListener);
  glutKeyboardFunc(keyboardListener);
  glutSpecialFunc(specialKeyListener);
  glutMouseFunc(mouseListener);
  initGL();
  glutMainLoop();
  return 0;
}