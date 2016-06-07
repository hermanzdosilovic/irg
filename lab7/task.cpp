#include <iostream>
#include <vector>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <float.h>

int g_width = 600, g_height = 600;

struct Triangle {
  glm::vec4 v1, v2, v3;
};

std::vector<glm::vec4> g_points, g_polygon;
std::vector<Triangle> g_triangles;

float g_min_x = FLT_MAX, g_min_y = FLT_MAX, g_min_z = FLT_MAX;
float g_max_x = FLT_MIN, g_max_y = FLT_MIN, g_max_z = FLT_MIN;

glm::vec4 g_camera = glm::vec4(20, 20, 20, 1),
          g_view = glm::vec4(0, 0, 0, 1),
          g_up = glm::vec4(0, 0, 1, 1);

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
glm::mat4 viewTransformMatrix(glm::vec4 camera, glm::vec4 view) {
  glm::mat4 T1 = glm::transpose(glm::mat4(
            1,         0,         0, 0,
            0,         1,         0, 0,
            0,         0,         1, 0,
    -camera.x, -camera.y, -camera.z, 1
  ));
  view = view * T1;


  double s;
  glm::mat4 T2;
  if (view.x || view.y) {
    s = sqrt(pow(view.x, 2) + pow(view.y, 2));
    double sinA = view.y / s;
    double cosA = view.x / s;
    T2 = glm::transpose(glm::mat4(
      cosA, -sinA,  0, 0,
      sinA,  cosA,  0, 0,
         0,     0,  1, 0,
         0,     0,  0, 1
    ));
  } else {
    T2 = glm::mat4();
  }
  view = view * T2;


  s = sqrt(pow(view.x, 2) + pow(view.z, 2));
  double cosB = view.z / s;
  double sinB = view.x / s;
  glm::mat4 T3 = glm::transpose(glm::mat4(
     cosB, 0, sinB, 0,
        0, 1,    0, 0,
    -sinB, 0, cosB, 0,
        0, 0,    0, 1
  ));
  view = view * T3;


  glm::mat4 T4 = glm::transpose(glm::mat4(
    0, -1, 0, 0,
    1,  0, 0, 0,
    0,  0, 1, 0,
    0,  0, 0, 1
  ));


  glm::mat4 T5 = glm::transpose(glm::mat4(
    -1, 0, 0, 0,
     0, 1, 0, 0,
     0, 0, 1, 0,
     0, 0, 0, 1
  ));

  glm::mat4 T = T1 * T2 * T3 * T4 * T5;
  return T;
}

glm::mat4 perspectiveTransformMatrix(glm::vec4 camera, glm::vec4 view) {
  double H = glm::distance(camera, view);
  return glm::transpose(glm::mat4(
    1, 0, 0,   0,
    0, 1, 0,   0,
    0, 0, 0, 1/H,
    0, 0, 0,   0
  ));
 }

 glm::mat4 scaleMatrix(float ratio) {
   return glm::transpose(glm::mat4(
     ratio,     0,     0, 0,
         0, ratio,     0, 0,
         0,     0, ratio, 0,
         0,     0,     0, 1
   ));
 }

void display() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT*/);

  float size_x = g_max_x - g_min_x;
  float size_y = g_max_y - g_min_y;
  float size_z = g_max_z - g_min_z;
  float center_x = (g_max_x + g_min_x)/2;
  float center_y = (g_max_y + g_min_y)/2;
  float center_z = (g_max_z + g_min_z)/2;
  float scale = 2/fmaxf(size_x, fmaxf(size_y, size_z));

  glPushMatrix();
  glScalef(scale, scale, scale);
  gluLookAt(g_camera.x, g_camera.y, g_camera.z, g_view.x, g_view.y, g_view.z, g_up.x, g_up.y, g_up.z);
  glTranslatef(-center_x, -center_y, -center_z);
  // glCullFace(GL_BACK);

  glm::mat4 S = scaleMatrix(scale);
  glm::mat4 T = viewTransformMatrix(g_camera, g_view);
  glm::mat4 P = perspectiveTransformMatrix(g_camera, g_view);
  glm::mat4 M = S*T*P;

  glColor3f(0.0f, 0.0f, 0.0f);
  glLineWidth(1);
  for (auto t : g_triangles) {
    glBegin(GL_LINES);
    glm::vec4 v1 = t.v1*M, v2 = t.v2*M, v3 = t.v3*M;
    glm::vec3 n = glm::cross(glm::vec3(v2 - v1), glm::vec3(v3 - v1));
    glm::vec4 c = (v1 + v2 + v3) / glm::vec4(3, 3, 3, 3);
    glm::vec3 p = glm::vec3(g_camera - c);
    double cosL = glm::dot(n, p)/glm::distance(n, p);
    if (cosL < 1e-6) {
      continue;
    }

    glVertex3f(t.v1.x, t.v1.y, t.v1.z);
    glVertex3f(t.v2.x, t.v2.y, t.v2.z);

    glVertex3f(t.v2.x, t.v2.y, t.v2.z);
    glVertex3f(t.v3.x, t.v3.y, t.v3.z);

    glVertex3f(t.v3.x, t.v3.y, t.v3.z);
    glVertex3f(t.v1.x, t.v1.y, t.v1.z);
    glEnd();
  }
  glPopMatrix();

  gluLookAt(g_camera.x, g_camera.y, g_camera.z, g_view.x, g_view.y, g_view.z, g_up.x, g_up.y, g_up.z);
  glLineWidth(2);
  glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0); glVertex3f(1, 0, 0);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0); glVertex3f(0, 1, 0);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0); glVertex3f(0, 0, 1);
  glEnd();

  glFlush();
}

void updatePerspective(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  gluPerspective(45.0, (float)width/height, 1, 100);
  // glFrustum(-1, 1, -1, 1, 1, 100);
}

void resize(int width, int height) {
  g_width = width;
  g_height = height;
  glViewport(0, 0, g_width, g_height);
  updatePerspective(width, height);
}

int g_motion_start = 0, g_x = 0;
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    g_motion_start = 1;
    g_x = x;
    glutPostRedisplay();
  } else if (button == 3 || button == 4) {
    g_motion_start = 0;
    glm::vec4 n = glm::normalize(g_view - g_camera);
    if (button == 3) {
      g_camera += n;
    } else {
      g_camera -= n;
    }
    glutPostRedisplay();
  } else {
    g_motion_start = 0;
  }
}

void motion(int x, int y) {
  if (!g_motion_start) {
    return;
  }
  float alpha = -(g_x - x) / glm::distance(g_camera, g_view);
  glm::mat4 R = glm::rotate(glm::mat4(), alpha, glm::vec3(g_up));
  g_camera = g_camera * R;
  g_up = g_up * R;
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
      g_min_x = fminf(g_min_x, x); g_max_x = fmaxf(g_max_x, x);
      g_min_y = fminf(g_min_y, y); g_max_y = fmaxf(g_max_y, y);
      g_min_z = fminf(g_min_z, z); g_max_z = fmaxf(g_max_z, z);
    } else if (line[0] == 'f') {
      int i, j, k;
      sscanf(&line[2], "%d %d %d", &i, &j, &k);
      g_triangles.push_back(Triangle{g_points[i - 1], g_points[j - 1], g_points[k - 1]});
    }
  }
  fclose(f);

  std::cout << "MIN: " << g_min_x << " " << g_min_y << " " << g_min_z << std::endl;
  std::cout << "MAX: " << g_max_x << " " << g_max_y << " " << g_max_z << std::endl;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(g_width, g_height);
  glutInitWindowPosition(700, 100);
  glutCreateWindow("Objects");
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  // glEnable(GL_CULL_FACE);
  // glEnable(GL_DEPTH_TEST);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glutMainLoop();

  return 0;
}
