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
std::vector<glm::vec4> g_planes;

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

int g_left_pressed, g_right_pressed;
int g_position_x, g_position_y;

void motion(int x, int y) {
  if (g_left_pressed) {
    g_translate_x += x - g_position_x;
    g_translate_y += y - g_position_y;
    g_position_x = x;
    g_position_y = y;
  } else if (g_right_pressed) {
    g_rotate_x += x - g_position_x;
    g_rotate_y += y - g_position_y;
    g_position_x = x;
    g_position_y = y;
  }
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    g_left_pressed = 1;
    g_right_pressed = 0;
    g_position_x = x;
    g_position_y = y;
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    g_left_pressed = 0;
    g_right_pressed = 1;
    g_position_x = x;
    g_position_y = y;
  } else if (button == 3 || button == 4) {
    g_scale += button == 3 ? 2 : -2;
    glutPostRedisplay();
  }
}

void resize(int width, int height) {
  g_width = width;
  g_height = height;
}

void calculatePlanes() {
  for(auto t : g_triangles) {
    glm::vec3 v = glm::cross(t.v2 - t.v1, t.v3 - t.v1);
    float D = -(v.x*t.v1.x + v.y*t.v1.y + v.z*t.v1.z);
    g_planes.push_back(glm::vec4(v.x, v.y, v.z, D));
    std::cout << v.x << ", " << v.y << ", " << v.z << std::endl;
  }
}

void calculatePointPosition(glm::vec4 point) {
  int is_inside = 1;
  for (auto v : g_planes) {
    if (glm::dot(v, point) > 0) {
      is_inside = 0;
      break;
    }
  }

  printf("Point (%f, %f, %f) is %s\n", point.x, point.y, point.z, is_inside ? "inside" : "outside");
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Expects two arguments: file with geometric and topologic data, file with test points.\n");
    return -1;
  }

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

  calculatePlanes();

  f = fopen(argv[2], "r");
  int number_of_points;
  fscanf(f, "%d", &number_of_points);
  while (number_of_points--) {
    float x, y, z;
    fscanf(f, "%f%f%f", &x, &y, &z);
    calculatePointPosition(glm::vec4(x, y, z, 1));
  }
  fclose(f);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(g_width, g_height);
  glutInitWindowPosition(200, 200);
  glutCreateWindow("Objects");
  glutDisplayFunc(display);
  glutMotionFunc(motion);
  glutMouseFunc(mouse);
  glutReshapeFunc(resize);
  glutMainLoop();

  return 0;
}
