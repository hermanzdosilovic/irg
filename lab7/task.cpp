#include <iostream>
#include <vector>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>

int g_width = 600, g_height = 600;

struct Triangle {
  glm::vec4 v1, v2, v3;
};

std::vector<glm::vec4> g_points, g_polygon;
std::vector<Triangle> g_triangles;

glm::vec4 g_camera = glm::vec4(20, 20, 20, 1),
          g_view = glm::vec4(0, 0, 0, 1);

std::ostream &operator<< (std::ostream &out, const glm::mat4 &m) {
  out << "{";
  for (int row = 0; row < 4; row++) {
    out << "(";
    for (int col = 0; col < 4; col++) {
      out << m[col][row];
      if (col + 1 != 4) out << ", ";
    }
    out << ")";
    if (row + 1 != 4) out << ", ";
  }
  out << "}";

  return out;
}

std::ostream &operator<< (std::ostream &out, const glm::vec4 &v) {
  out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
  return out;
}


void display() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
  gluLookAt(g_camera.x, g_camera.y, g_camera.z, g_view.x, g_view.y, g_view.z, 0, 1, 0);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glScalef(10, 10, 10);

  std::cout << g_camera << std::endl;

  glColor3f(0.0f, 0.0f, 0.0f);
  glLineWidth(1);
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

  glLineWidth(3);
  glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0); glVertex3f(0.2, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0); glVertex3f(0, 0.2, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0); glVertex3f(0, 0, 0.2);
  glEnd();

  glFlush();
}

void resize(int width, int height) {
  g_width = width;
  g_height = height;
  glViewport(0, 0, g_width, g_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  glFrustum(-1, 1, -1, 1, glm::distance(g_camera, g_view)/10, 10*glm::distance(g_camera, g_view));
}

int g_motion_start = 0, g_x = 0;
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    g_motion_start = 1;
    g_x = x;
  } else {
    g_motion_start = 0;
    glm::vec4 n = glm::normalize(g_view - g_camera);
    if (button == 3) {
      g_camera += n;
    } else if (button == 4) {
      g_camera -= n;
    }
  }
  glutPostRedisplay();
}

void motion(int x, int y) {
  if (!g_motion_start) {
    return;
  }
  double alpha = - (g_x - x) / glm::distance(g_camera, glm::vec4(0, 0, 0, 1));

  glm::mat4 Z = glm::transpose(glm::mat4(
     cos(alpha), 0, sin(alpha), 0,
              0, 1,          0, 0,
    -sin(alpha), 0, cos(alpha), 0,
              0, 0,          0, 1
  ));
  g_camera = g_camera * Z;
  g_x = x;

  glutPostRedisplay();
}

int main(int argc, char **argv) {
  FILE *f = fopen(argv[1], "r");
  char line[100];
  while (fscanf(f, "%[^\n] ", line) != EOF) {
    if (line[0] == 'v') {
      float x, y, z;
      sscanf(&line[2], "%f %f %f", &x, &y, &z);
      g_points.push_back(glm::vec4(x, y, z, 1));
    } else if (line[0] == 'f') {
      int i, j, k;
      sscanf(&line[2], "%d %d %d", &i, &j, &k);
      g_triangles.push_back(Triangle{g_points[i - 1], g_points[j - 1], g_points[k - 1]});
    }
  }
  fclose(f);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(g_width, g_height);
  glutInitWindowPosition(700, 100);
  glutCreateWindow("Objects");
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMainLoop();

  return 0;
}
