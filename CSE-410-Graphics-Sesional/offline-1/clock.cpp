#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <bits/stdc++.h>
using namespace std;

#ifdef __linux__
#include <GL/glut.h>
#else WIN32
#include <glut.h>
#include <windows.h>
#endif

#define PI 2 * acos(0.0)

// Draw a circle at (cx, cy) with radius r
void drawCircle(float cx, float cy, float r, int num_segments) {
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < num_segments; i++) {
    float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
    float x = r * cosf(theta);
    float y = r * sinf(theta);
    glVertex2f(x + cx, y + cy);
  }
  glEnd();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  using namespace std::chrono;
  auto now = system_clock::now();
  auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
  time_t t = system_clock::to_time_t(now);
  tm *ltm = localtime(&t);

  double angleM, angleS, angleH;
  double r = 0.85;

  // Get current time with fractions of a second
  double seconds = ltm->tm_sec + ms.count() / 1000.0;
  double minutes = ltm->tm_min + seconds / 60.0;
  double hours = (ltm->tm_hour % 12) + minutes / 60.0;

  // Calculate angles for hour, minute, and second hands (clockwise)
  angleS = seconds * 6.0; // 360/60
  angleM = minutes * 6.0; // 360/60
  angleH = hours * 30.0;  // 360/12

  double xS = r * cos((-angleS + 90) * PI / 180.0);
  double yS = r * sin((-angleS + 90) * PI / 180.0);
  double xM = r * cos((-angleM + 90) * PI / 180.0);
  double yM = r * sin((-angleM + 90) * PI / 180.0);
  double xH = r * cos((-angleH + 90) * PI / 180.0);
  double yH = r * sin((-angleH + 90) * PI / 180.0);

  glColor3f(1, 1, 1);
  drawCircle(0, 0, r, 200);

  glColor3f(1, 1, 1);
  drawCircle(0, 0, r, 200);

  // Draw hour indicators (numbers 1-12)
  for (int i = 1; i <= 12; ++i) {
    float angle =
        -(i - 3) * 30.0f * PI / 180.0f; // -3 to rotate so 12 is at top
    float tx = (r - 0.12f) * cos(angle);
    float ty = (r - 0.12f) * sin(angle);

    glRasterPos2f(tx - 0.025f, ty - 0.025f); // Adjust for centering
    std::string num = std::to_string(i);
    for (char c : num) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
  }

  // Draw 60 tick marks around the clock face
  for (int i = 0; i < 60; ++i) {
    float angle = i * 6.0f * PI / 180.0f; // 360/60 = 6 degrees per tick
    float xStart = (r - 0.02f) * cos(angle);
    float yStart = (r - 0.02f) * sin(angle);
    float xEnd = r * cos(angle);
    float yEnd = r * sin(angle);

    if (i % 5 == 0) { // Hour tick (every 5th)
      xStart = (r - 0.06f) * cos(angle);
      yStart = (r - 0.06f) * sin(angle);
      glLineWidth(3.0f);
    } else {
      glLineWidth(1.0f);
    }

    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(xStart, yStart);
    glVertex2f(xEnd, yEnd);
    glEnd();
  }
  glLineWidth(1.0f); // Reset to default

  glLineWidth(4.0f); // Set line width to make it thicker
  glBegin(GL_LINES);
  glColor3f(1, 1, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(xH * 0.45, yH * 0.45, 0);
  glEnd();

  glLineWidth(1.0f); // Reset to default for other lines
  glBegin(GL_LINES);
  glColor3f(1, 1, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(xM * 0.65, yM * 0.6, 0);
  glEnd();

  glLineWidth(1.0f); // Reset to default for other lines
  glBegin(GL_LINES);
  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(xS * 0.75, yS * 0.75, 0);
  glEnd();

  glColor3f(0, 1, 0); // Green quadrilateral
  glBegin(GL_QUADS);
  // Draw a small quadrilateral at the tip of the second hand on the circle
  float quadSize = 0.02f; // Size of the quadrilateral
  float angleRad = (-angleS + 90) * PI / 180.0;
  float cx = r * cos(angleRad);
  float cy = r * sin(angleRad);

  // Offset the quad so it's centered at (cx, cy)
  float dx = quadSize * cos(angleRad + PI / 2);
  float dy = quadSize * sin(angleRad + PI / 2);

  glVertex2f(cx - dx, cy - dy);
  glVertex2f(cx + dx, cy + dy);
  glVertex2f(cx + dx + quadSize * cos(angleRad),
             cy + dy + quadSize * sin(angleRad));
  glVertex2f(cx - dx + quadSize * cos(angleRad),
             cy - dy + quadSize * sin(angleRad));
  glEnd();
  glFlush();
}

int main(int argc, char **argv) {
  cout << "Hello World!" << endl;
  glutInit(&argc, argv);
  glutInitWindowPosition(400, 50);
  glutInitWindowSize(900, 900);
  glutCreateWindow("Tamim");

  // init();
  glutDisplayFunc(display);
  glutIdleFunc(display);

  glutMainLoop();
  return 0;
}