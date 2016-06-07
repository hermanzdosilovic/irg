#include <GL/freeglut.h>
#include <iostream>

int g_width = 500, g_height = 500;

double g_epsilon, g_u_min, g_u_max, g_v_min, g_v_max;
int g_max_iterations;

int divergence_test(double re, double im, int limit) {
  double _re = 0, _im = 0;
  for (int i = 1; i <= limit; i++) {
    double next_re = _re*_re - _im*_im + re;
    double next_im = 2*_re*_im + im;
    _re = next_re;
    _im = next_im;
    double modul2 = _re*_re + _im*_im;
    if (modul2 > 4) return i;
  }
  return -1;
}

void display() {
  glViewport(0, 0, g_width, g_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, g_width, 0, g_height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glPointSize(1);
  glBegin(GL_POINTS);
  for (int y = 0; y < g_height; y++) {
    for (int x = 0; x < g_width; x++) {
        double u = (double)(g_u_max - g_u_min)*x/g_width + g_u_min;
        double v = (double)(g_v_max - g_v_min)*y/g_height + g_v_min;

        int n = divergence_test(u, v, g_max_iterations);
        if (n == -1) {
          glColor3f(0, 0, 0);
        } else {
          glColor3f((double)n/g_max_iterations, 1-(double)n/g_max_iterations/2.0, 0.8-(double)n/g_max_iterations/3.0);
        }
        glVertex2i(x, y);
    }
  }
  glEnd();

  glFlush();
}

int main(int argc, char **argv) {
  FILE *f = fopen(argv[1], "r");
  fscanf(f, "%lf", &g_epsilon);
  fscanf(f, "%d", &g_max_iterations);
  fscanf(f, "%lf %lf %lf %lf", &g_u_min, &g_u_max, &g_v_min, &g_v_max);
  fscanf(f, "%d %d", &g_width, &g_height);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(g_width, g_height);
  glutInitWindowPosition(700, 100);

  glutCreateWindow("Objects");
  glutDisplayFunc(display);
  glutMainLoop();

  return 0;
}