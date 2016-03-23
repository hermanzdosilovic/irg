#include <stdio.h>
#include <freeglut.h>

struct Point {
  int x, y;
} T[2];

int idx; // remember when to draw a line

const int WIDTH = 500, HEIGHT = 500;

void bresenham() {
  double k = (T[1].y - T[0].y)/(double)(T[1].x - T[0].x);
  int yc = T[0].y;
  double yf = -0.5;

  printf("%f\n", k);
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_POINTS);
  for (int xi = T[0].x; xi <= T[1].x; xi++) {
    glVertex2i(xi, yc);
    yf += k;
    if (yf >= 0) {
      yc++;
      yf -= 1;
    }
  }
  glEnd();
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

void mouse(int button, int state, int x, int y) {
  if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) {
    return;
  }

  T[idx] = {x, y};
  idx = (idx + 1)%2;

  if (idx != 0) {
    return; // wait for another point
  }
  
  
  printf("Line from (%d, %d) to (%d, %d)\n",
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

