#include <stdio.h>
#include <freeglut.h>

struct Point {
  int x, y;
} T[2];

int idx; // remember when to draw a line

const int WIDTH = 500, HEIGHT = 500;

void bresenham() {
  if (T[0].x > T[1].x) {
    Point X = T[0];
    T[0] = T[1];
    T[1] = X;
  }
  
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_POINTS);

  double dx = (T[1].x - T[0].x);
  double dy = (T[1].y - T[0].y);
  
  if (dx >= dy && dy > 0) {
    printf("[0, 45]\n");
    double k = dy/dx;
    int yc = T[0].y;
    double yf = -0.5;
    for (int xi = T[0].x; xi <= T[1].x; xi++) {
      glVertex2i(xi, yc);
      yf += k;
      if (yf >= 0) {
        yc++;
        yf -= 1;
      }
    }
  } else if (dx < dy && dy > 0) {
    printf("(45, 90]\n");
    T[0] = {T[0].y, T[0].x};
    T[1] = {T[1].y, T[1].x};
    double k = dx/dy;
    int yc = T[0].y;
    double yf = -0.5;
    for (int xi = T[0].x; xi <= T[1].x; xi++) {
      glVertex2i(yc, xi);
      yf += k;
      if (yf >= 0) {
        yc++;
        yf -= 1;
      }
    }
  } else if (dx >= -dy) {
    printf("[-45, 0)\n");
    double k = dy/dx;
    int yc = T[0].y;
    double yf = 0.5;
    for (int xi = T[0].x; xi <= T[1].x; xi++) {
      glVertex2i(xi, yc);
      yf += k;
      if (yf <= 0) {
        yc--;
        yf += 1;
      }
    }
  } else {
    printf("[-90, -45)\n");
    T[0] = {T[0].y, T[0].x};
    T[1] = {T[1].y, T[1].x};
    if (T[0].x > T[1].x) {
      Point X = T[0];
      T[0] = T[1];
      T[1] = X;
    }
    double k = dx/dy;
    int yc = T[0].y;
    double yf = 0.5;
    for (int xi = T[0].x; xi <= T[1].x; xi++) {
      glVertex2i(yc, xi);
      yf += k;
      if (yf <= 0) {
        yc--;
        yf += 1;
      }
    }
  }
  glEnd();
}

void mouse(int button, int state, int x, int y) {
  if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) {
    return;
  }

  T[idx] = {x, y};
  idx = (idx + 1)%2;

  if (idx != 0) {
    return; // wait for another point
  }
  
  
  printf("Line from (%d, %d) to (%d, %d) ",
    T[0].x, T[0].y, T[1].x, T[1].y);
  
  // built-in function
  glPointSize(1.0);
  glColor3f(0.0f, 0.0f, 0.0f);
  glBegin(GL_LINES);
  glVertex2i(T[0].x, T[0].y + 20);
  glVertex2i(T[1].x, T[1].y + 20);
  glEnd();

  // bresenham's algorithm
  bresenham();

  glFlush();
}

void display() {
  glViewport(0, 0, WIDTH, HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, WIDTH, HEIGHT, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(200, 200); 
  
  glutCreateWindow("Bresenham");
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutMainLoop();

  return 0;
}

