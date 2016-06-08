#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

int g_width = 500, g_height = 500;
double g_epsilon, g_complex_const_re, g_complex_const_im, g_u_min, g_u_max, g_v_min, g_v_max;
int g_max_iterations;
int g_fractal_mode = 0;
const int ZOOM_IN = 1, ZOOM_OUT = -1;

int divergence_test(double re, double im, int limit) {
  double _re, _im;
  if (!g_fractal_mode) { // Mandelbrot
    _re = 0;
    _im = 0;
  } else { // Julia
    _re = re;
    _im = im;
    re = g_complex_const_re;
    im = g_complex_const_im;
  }
  for (int i = 1; i <= limit; i++) {
    double next_re = _re*_re - _im*_im + re;
    double next_im = 2*_re*_im + im;
    _re = next_re;
    _im = next_im;
    double modul2 = _re*_re + _im*_im;
    if (modul2 > g_epsilon) {
      return i;
    }
  }
  return -1;
}

double complex_x(int x) {
  return (double)(g_u_max - g_u_min)*x/g_width + g_u_min;
}

double complex_y(int y) {
  return (double)(g_v_max - g_v_min)*y/g_height + g_v_min;
}

void scale_complex_plane(int f) { // f is 1 or -1
    double d;

    d = (g_u_max - g_u_min)/10;
    g_u_min -= f*d;
    g_u_max += f*d;

    d = (g_v_max - g_v_min)/10;
    g_v_min -= f*d;
    g_v_max += f*d;
}

void center_complex_plane(int x, int y) {
    double u = complex_x(x);
    double v = complex_y(y);
    double center_u = complex_x(g_width/2);
    double center_v = complex_y(g_height/2);
    double du = center_u - u;
    double dv = center_v - v;

    g_u_min -= du; g_u_max -= du;
    g_v_min += dv; g_v_max += dv;
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
      double u = complex_x(x);
      double v = complex_y(y);

      int n = divergence_test(u, v, g_max_iterations);
      if (n == -1) {
        glColor3f(0, 0, 0);
      } else {
        glColor3f(1.0*n/g_max_iterations, 3.0*n/g_max_iterations, 8.0*n/g_max_iterations);
      }
      glVertex2i(x, y);
    }
  }
  glEnd();

  glFlush();
}

void keyboard(unsigned char key, int x, int y) {
  if (key == 'n' || key == 'N') {
    g_fractal_mode = (g_fractal_mode + 1)%2;
    glutPostRedisplay();
  }
}

void resize(int width, int height) {
  g_width = width;
  g_height = height;
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    center_complex_plane(x, y);
    glutPostRedisplay();
  } else if (button == 3) {
    scale_complex_plane(ZOOM_IN);
    glutPostRedisplay();
  } else if (button == 4) {
    scale_complex_plane(ZOOM_OUT);
    glutPostRedisplay();
  }
}

int main(int argc, char **argv) {
  FILE *f = fopen(argv[1], "r");
  fscanf(f, "%lf", &g_epsilon);
  fscanf(f, "%d", &g_max_iterations);
  fscanf(f, "%lf %lf %lf %lf", &g_u_min, &g_u_max, &g_v_min, &g_v_max);
  fscanf(f, "%d %d", &g_width, &g_height);
  fscanf(f, "%lf %lf", &g_complex_const_re, &g_complex_const_im);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(g_width, g_height);
  glutInitWindowPosition(700, 100);

  glutCreateWindow("Fractals");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutReshapeFunc(resize);
  glutMainLoop();

  return 0;
}
