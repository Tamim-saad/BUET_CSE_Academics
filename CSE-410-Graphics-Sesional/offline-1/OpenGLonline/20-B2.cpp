#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

// Camera vectors and movement step
float camX = 1.5, camY = 1.5, camZ = 1.5;
float targetX = 0, targetY = 0, targetZ = 0;
float upX = 0, upY = 1, upZ = 0;
float step = 0.2f;

// Ball properties and physics
float ballX = 0.0f, ballY = -0.7f + 0.08f, ballZ = 0.0f; // Ball position
float ballRadius = 0.08f;
float ballSpeed = 0.0f;                                  // Ball speed
float ballDirX = 1.0f, ballDirY = 0.0f, ballDirZ = 0.0f; // Ball direction
float ballRotationAngle = 0.0f; // Ball rolling rotation angle
float ballRotationAxisX = 0.0f, ballRotationAxisY = 0.0f,
      ballRotationAxisZ = 1.0f; // Rotation axis

// Gravity and bounce
float ballVelY = 0.0f;                  // Vertical velocity
float gravity = -9.8f;                  // Gravity acceleration
float restitution = 0.75f;              // Bounciness
float groundLevel = -0.7f + ballRadius; // Ground level

bool simulationRunning = false; // Simulation state
bool showVelocityArrow = false; // Show velocity arrow

// Normalize a 3D vector (in-place)
void normalize(float &x, float &y, float &z) {
  float len = sqrt(x * x + y * y + z * z);
  if (len != 0) {
    x /= len;
    y /= len;
    z /= len;
  }
}

// Draw the ball with alternating red and green stripes
void drawBall() {
  glPushMatrix();
  glTranslatef(ballX, ballY, ballZ);
  glRotatef(ballRotationAngle, ballRotationAxisX, ballRotationAxisY,
            ballRotationAxisZ);

  int slices = 50, stacks = 50;
  for (int i = 0; i < slices; i++) {
    float theta1 = (i * 2.0f * M_PI) / slices;
    float theta2 = ((i + 1) * 2.0f * M_PI) / slices;

    glColor3f(i % 2 == 0 ? 1.0f : 0.0f, i % 2 == 0 ? 0.0f : 1.0f, 0.0f);

    glBegin(GL_QUAD_STRIP);
    for (int j = 0; j <= stacks; j++) {
      float phi = (j * M_PI) / stacks;
      float c = cos(phi), s = sin(phi);

      glVertex3f(ballRadius * s * cos(theta1), ballRadius * c,
                 ballRadius * s * sin(theta1));
      glVertex3f(ballRadius * s * cos(theta2), ballRadius * c,
                 ballRadius * s * sin(theta2));
    }
    glEnd();
  }
  glPopMatrix();
}

// Draws a yellow arrow representing the ball's velocity
void drawVelocityArrow() {
  if (!showVelocityArrow || ballSpeed == 0.0f)
    return;

  glColor3f(1.0f, 1.0f, 0.0f);
  glPushMatrix();
  glTranslatef(ballX, ballY, ballZ);

  float vx = ballDirX, vy = ballVelY, vz = ballDirZ;
  normalize(vx, vy, vz);
  float fixedLength = 0.25f;
  vx *= fixedLength;
  vy *= fixedLength;
  vz *= fixedLength;

  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(vx, vy, vz);
  glEnd();

  glPushMatrix();
  glTranslatef(vx, vy, vz);

  float len = sqrtf(vx * vx + vy * vy + vz * vz);
  if (len > 1e-4f) {
    float angle = acosf(vz / len) * 180.0f / M_PI;
    float ax = -vy, ay = vx, az = 0.0f;
    if (fabs(vz / len) < 0.99999f)
      glRotatef(angle, ax, ay, az);
  }
  glutSolidCone(0.04, 0.05, 10, 10);
  glPopMatrix();

  glPopMatrix();
}

// Reset ball to random position, direction, and speed
void resetBall() {
  ballDirY = 0.0f;
  float boundaryX = 1.0f - ballRadius, boundaryZ = 1.0f - ballRadius;
  ballX = ((rand() / (float)RAND_MAX) * 2 - 1) * boundaryX;
  ballZ = ((rand() / (float)RAND_MAX) * 2 - 1) * boundaryZ;
  ballY = groundLevel;
  ballSpeed = (rand() % 10 + 1) / 500.0f;
  ballDirX = (rand() % 200 - 100) / 100.0f;
  ballDirZ = (rand() % 200 - 100) / 100.0f;
  normalize(ballDirX, ballDirZ, ballDirZ);
  ballVelY = 2.0f;
  ballRotationAngle = 0.0f;
}

// Helper to handle bounce at ±boundary on one axis
static void handleBoundary(float &pos, float &dir, float boundary, float &speed,
                           float restitution) {
  if (pos > boundary) {
    pos = boundary;
    dir = -dir;
    speed *= restitution;
  } else if (pos < -boundary) {
    pos = -boundary;
    dir = -dir;
    speed *= restitution;
  }
}

// Update ball position, rotation, and physics (gravity, bounce, boundaries)
void updateBall() {
  if (!simulationRunning)
    return;

  const float dt = 0.016f;  // fixed time step
  ballVelY += gravity * dt; // gravity
  ballY += ballVelY * dt;   // vertical move

  // ground collision & bounce
  if (ballY <= groundLevel) {
    ballY = groundLevel;
    if (fabs(ballVelY) > 0.1f)
      ballVelY = -ballVelY * restitution;
    else
      ballVelY = 0.0f;
  }

  // horizontal move
  float displacement = ballSpeed;
  ballX += ballDirX * displacement;
  ballZ += ballDirZ * displacement;

  // boundary collisions
  float bx = 1.0f - ballRadius;
  float bz = 1.0f - ballRadius;
  handleBoundary(ballX, ballDirX, bx, ballSpeed, restitution);
  handleBoundary(ballZ, ballDirZ, bz, ballSpeed, restitution);

  // rolling rotation
  float rotationAngle = (displacement / ballRadius) * (180.0f / M_PI);
  ballRotationAngle += rotationAngle;

  // update axis
  ballRotationAxisX = ballDirZ;
  ballRotationAxisZ = -ballDirX;
  normalize(ballRotationAxisX, ballRotationAxisY, ballRotationAxisZ);
}

// Handle special key events (arrow keys, page up/down) for camera movement
void specialKeys(int key, int x, int y) {
  // Compute forward vector (target - camera)
  float fx = targetX - camX;
  float fy = targetY - camY;
  float fz = targetZ - camZ;
  normalize(fx, fy, fz); // Forward direction

  // Compute right vector = forward x up
  float rx = fy * upZ - fz * upY;
  float ry = fz * upX - fx * upZ;
  float rz = fx * upY - fy * upX;
  normalize(rx, ry, rz);

  // Move camera and target based on key
  if (key == GLUT_KEY_UP) {
    camX += fx * step;
    camY += fy * step;
    camZ += fz * step;
    targetX += fx * step;
    targetY += fy * step;
    targetZ += fz * step;
  }
  if (key == GLUT_KEY_DOWN) {
    camX -= fx * step;
    camY -= fy * step;
    camZ -= fz * step;
    targetX -= fx * step;
    targetY -= fy * step;
    targetZ -= fz * step;
  }
  if (key == GLUT_KEY_LEFT) {
    camX -= rx * step;
    targetX -= rx * step;
    camY -= ry * step;
    targetY -= ry * step;
    camZ -= rz * step;
    targetZ -= rz * step;
  }
  if (key == GLUT_KEY_RIGHT) {
    camX += rx * step;
    targetX += rx * step;
    camY += ry * step;
    targetY += ry * step;
    camZ += rz * step;
    targetZ += rz * step;
  }
  if (key == GLUT_KEY_PAGE_UP) {
    camY += step;
    targetY += step;
  }
  if (key == GLUT_KEY_PAGE_DOWN) {
    camY -= step;
    targetY -= step;
  }

  glutPostRedisplay();
}

// Rotate the camera around a given axis (angle in degrees)
void rotateCamera(float angle, float x, float y, float z) {
  // normalize axis
  float lenA = sqrtf(x * x + y * y + z * z);
  if (lenA < 1e-6f)
    return;
  x /= lenA;
  y /= lenA;
  z /= lenA;

  float rad = angle * M_PI / 180.0f;
  float cosA = cosf(rad), sinA = sinf(rad);

  // rotate target vector
  float vX = targetX - camX, vY = targetY - camY, vZ = targetZ - camZ;
  float rX = vX * (cosA + (1 - cosA) * x * x) +
             vY * ((1 - cosA) * x * y - sinA * z) +
             vZ * ((1 - cosA) * x * z + sinA * y);
  float rY = vX * ((1 - cosA) * y * x + sinA * z) +
             vY * (cosA + (1 - cosA) * y * y) +
             vZ * ((1 - cosA) * y * z - sinA * x);
  float rZ = vX * ((1 - cosA) * z * x - sinA * y) +
             vY * ((1 - cosA) * z * y + sinA * x) +
             vZ * (cosA + (1 - cosA) * z * z);
  targetX = camX + rX;
  targetY = camY + rY;
  targetZ = camZ + rZ;

  // rotate up vector
  float uX = upX, uY = upY, uZ = upZ;
  float ruX = uX * (cosA + (1 - cosA) * x * x) +
              uY * ((1 - cosA) * x * y - sinA * z) +
              uZ * ((1 - cosA) * x * z + sinA * y);
  float ruY = uX * ((1 - cosA) * y * x + sinA * z) +
              uY * (cosA + (1 - cosA) * y * y) +
              uZ * ((1 - cosA) * y * z - sinA * x);
  float ruZ = uX * ((1 - cosA) * z * x - sinA * y) +
              uY * ((1 - cosA) * z * y + sinA * x) +
              uZ * (cosA + (1 - cosA) * z * z);
  upX = ruX;
  upY = ruY;
  upZ = ruZ;
}

// Handle keyboard input for camera and simulation controls
void keyboard(unsigned char key, int x, int y) {
  if (key == 'w') {
    camY += step;
  } else if (key == 's') {
    camY -= step;
  }
  // Yaw: world‐up axis
  else if (key == '1' || key == '2') {
    rotateCamera(key == '1' ? -5.0f : 5.0f, 0, 1, 0);
  }
  // Pitch: axis = camera right vector
  else if (key == '3' || key == '4') {
    // forward = target - cam
    float fx = targetX - camX, fy = targetY - camY, fz = targetZ - camZ;
    normalize(fx, fy, fz);
    // right = forward × up
    float rx = fy * upZ - fz * upY;
    float ry = fz * upX - fx * upZ;
    float rz = fx * upY - fy * upX;
    normalize(rx, ry, rz);
    rotateCamera(key == '3' ? -5.0f : 5.0f, rx, ry, rz);
  }
  // Roll: axis = forward vector
  else if (key == '5' || key == '6') {
    float fx = targetX - camX, fy = targetY - camY, fz = targetZ - camZ;
    normalize(fx, fy, fz);
    rotateCamera(key == '5' ? 5.0f : -5.0f, fx, fy, fz);
  } else if (key == ' ') {
    simulationRunning = !simulationRunning;
  } else if (key == 'r') {
    resetBall();
  } else if (key == '+') {
    ballSpeed += 0.01f;
  } else if (key == '-') {
    ballSpeed = max(0.0f, ballSpeed - 0.01f);
  } else if (key == 'v') {
    showVelocityArrow = !showVelocityArrow;
  }
  glutPostRedisplay();
}

// Render the scene (draws a colored cube with a checkerboard bottom and the
// ball)
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(camX, camY, camZ, targetX, targetY, targetZ, upX, upY, upZ);

  // Draw cube faces and checkerboard
  glBegin(GL_QUADS);

  // +X face (right) - Red
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(1.0, -0.7, -1.0);
  glVertex3f(1.0, 0.7, -1.0);
  glVertex3f(1.0, 0.7, 1.0);
  glVertex3f(1.0, -0.7, 1.0);

  // -X face (left) - Red-light
  glColor3f(1.0, 0.5, 0.5);
  glVertex3f(-1.0, -0.7, -1.0);
  glVertex3f(-1.0, -0.7, 1.0);
  glVertex3f(-1.0, 0.7, 1.0);
  glVertex3f(-1.0, 0.7, -1.0);

  // +Y face (top) - Green
  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(-1.0, 0.7, -1.0);
  glVertex3f(-1.0, 0.7, 1.0);
  glVertex3f(1.0, 0.7, 1.0);
  glVertex3f(1.0, 0.7, -1.0);

  // -Y face (bottom) - Checkerboard
  for (int i = 0; i < 22; i++) {
    for (int j = 0; j < 22; j++) {
      if ((i + j) % 2 == 0)
        glColor3f(1.0, 1.0, 1.0); // White
      else
        glColor3f(0.0, 0.0, 0.0); // Black

      float x1 = -1.0 + i * (2.0 / 22);
      float x2 = -1.0 + (i + 1) * (2.0 / 22);
      float z1 = -1.0 + j * (2.0 / 22);
      float z2 = -1.0 + (j + 1) * (2.0 / 22);

      glVertex3f(x1, -0.7, z1);
      glVertex3f(x2, -0.7, z1);
      glVertex3f(x2, -0.7, z2);
      glVertex3f(x1, -0.7, z2);
    }
  }

  // +Z face (front) - Blue
  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(-1.0, -0.7, 1.0);
  glVertex3f(1.0, -0.7, 1.0);
  glVertex3f(1.0, 0.7, 1.0);
  glVertex3f(-1.0, 0.7, 1.0);

  // -Z face (back) - Blue-light
  glColor3f(0.5, 0.5, 1.0);
  glVertex3f(-1.0, -0.7, -1.0);
  glVertex3f(-1.0, 0.7, -1.0);
  glVertex3f(1.0, 0.7, -1.0);
  glVertex3f(1.0, -0.7, -1.0);

  glEnd();

  // Draw ball
  drawBall();

  // Draw velocity arrow AFTER the ball and cube
  drawVelocityArrow();

  glutSwapBuffers(); // Use swap buffers with double buffering
}

// Timer function for updating the ball and refreshing display
void timer(int value) {
  updateBall();
  glutPostRedisplay();
  glutTimerFunc(16, timer, 0); // Call every 16ms (~60 FPS)
}

// Initialize OpenGL settings and perspective
void init() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90, 1.0, 0.1, 100.0);
}

// Main entry point: sets up GLUT, OpenGL, and starts main loop
int main(int argc, char **argv) {
  srand(time(0)); // Seed random number generator
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Rolling Ball");
  init();
  glutDisplayFunc(display);
  glutSpecialFunc(specialKeys);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(16, timer, 0); // Start timer

  // Print controls to terminal
  cout << "\n--- 3D Bouncing Ball Controls ---\n"
          "Space: Toggle simulation on/off\n"
          "v: Toggle velocity arrow display\n"
          "+/-: Increase/decrease ball speed (when paused)\n"
          "r: Reset ball to random position (when paused)\n\n"
          "--- Camera Controls ---\n"
          "Arrow keys: Move camera left/right/forward/backward\n"
          "Page Up/Down: Move camera up/down\n"
          "w/s: Move camera up/down (keeping reference point)\n"
          "1/2: Look left/right\n"
          "3/4: Look up/down\n"
          "5/6: Tilt camera clockwise/counterclockwise\n\n";

  glutMainLoop();
  return 0;
}
