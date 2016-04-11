#include <iostream>
#include <GL/freeglut.h>

int g_width = 600, g_height = 600;

void display() {
  glViewport(0, 0, g_width, g_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, g_width, g_height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

void resize(int width, int height) {
  g_width = width;
  g_height = height;
}

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("Expected one argument: file with geometric and topologic data.\n");
    return 0;
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(200, 200);

  glutCreateWindow("Objects");

  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMainLoop();
  return 0;
}
