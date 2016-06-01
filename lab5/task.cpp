#include <iostream>
#include <vector>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>

int g_width = 600, g_height = 600;
int g_translate_x = g_width/2, g_translate_y = g_height/2;
int g_scale = 10;
int g_rotate_x = 0, g_rotate_y = 0;

struct Triangle {
  glm::vec3 v1, v2, v3;
};

std::vector<glm::vec3> g_points;
std::vector<Triangle> g_triangles;

void display() {
  glViewport(0, 0, g_width, g_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  glOrtho(0, g_width, g_height, 0, -1000, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glTranslatef(g_translate_x, g_translate_y, 0);
  glScalef(g_scale, g_scale, g_scale);

  glRotatef(g_rotate_x, 0.0, 1.0, 0.0);
  glRotatef(g_rotate_y, 1.0, 0.0, 0.0);

  glColor3f(0.0f, 0.0f, 0.0f);
  glBegin(GL_LINES);
  for (auto t : g_triangles) {
    glVertex3f(t.v1.x, t.v1.y, t.v1.z);
    glVertex3f(t.v2.x, t.v2.y, t.v2.z);

    glVertex3f(t.v2.x, t.v2.y, t.v2.z);
    glVertex3f(t.v3.x, t.v3.y, t.v3.z);

    glVertex3f(t.v3.x, t.v3.y, t.v3.z);
    glVertex3f(t.v1.x, t.v1.y, t.v1.z);
  }
  glEnd();

  glFlush();
}

void resize(int width, int height) {
  g_width = width;
  g_height = height;
}

int main(int argc, char **argv) {
//  if (argc != 2) {
//    printf("Expects two arguments: file with geometric and topologic data, file with viewpoints\n");
//    return -1;
//  }

  FILE *f = fopen(argv[1], "r");
  char line[100];
  while (fscanf(f, "%[^\n] ", line) != EOF) {
    if (line[0] == 'v') {
      float x, y, z;
      sscanf(&line[2], "%f %f %f", &x, &y, &z);
      g_points.push_back(glm::vec3(x, y, z));
    } else if (line[0] == 'f') {
      int i, j, k;
      sscanf(&line[2], "%d %d %d", &i, &j, &k);
      g_triangles.push_back(Triangle{g_points[i - 1], g_points[j - 1], g_points[k - 1]});
    }
  }
  fclose(f);

//  f = fopen(argv[2], "r");
//  fclose(f);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(g_width, g_height);
  glutInitWindowPosition(200, 200);
  glutCreateWindow("Objects");
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMainLoop();

  return 0;
}
