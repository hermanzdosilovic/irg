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

std::vector<glm::vec4> g_points, g_polygon, g_bezier_points;
std::vector<Triangle> g_triangles;

glm::vec4 g_camera = glm::vec4(400, 400, 400, 1), g_view = glm::vec4(100, 100, 100, 1);

int g_bezier_index = -1;

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

glm::vec4 transformPoint(glm::vec4 point, glm::mat4 matrix) {
  glm::vec4 transformedPoint = point * matrix;
  return transformedPoint / transformedPoint.w;
}

std::vector<glm::vec4> calculateBezierPoints(std::vector<glm::vec4> polygon, int divs) {
    std::vector<double> factors;
    int a = 1, n = polygon.size() - 1;

    for (int i = 1; i <= n + 1; i++) {
      factors.push_back(a);
      a = a * (n - i + 1) / i;
    }

    std::vector<glm::vec4> points;
    glm::vec4 p;
    double t, b;
    for (int i = 0; i <= divs; i++) {
      t = 1.0/divs*i;
      p.x = 0, p.y = 0, p.z = 0, p.w = 1;
      for (int j = 0; j <= n; j++) {
        if (j == 0) {
          b = factors[j]*pow(1 - t, n);
        } else if (j == n) {
          b = factors[j]*pow(t, n);
        } else {
          b = factors[j]*pow(t, j)*pow(1 - t, n - j);
        }
        p.x += b*polygon[j].x;
        p.y += b*polygon[j].y;
        p.z += b*polygon[j].z;
      }
      points.push_back(p);
    }

    return points;
}

void display() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 T = viewTransformMatrix(g_camera, g_view);
  glm::mat4 P = perspectiveTransformMatrix(g_camera, g_view);
  glm::mat4 S = scaleMatrix(200);
  glm::mat4 transformMatrix = S * T * P;

  glColor3f(0.0f, 0.0f, 0.0f);
  glLineWidth(1);
  glBegin(GL_LINES);
  for (auto t : g_triangles) {
    glm::vec4 v1 = transformPoint(t.v1, transformMatrix);
    glm::vec4 v2 = transformPoint(t.v2, transformMatrix);
    glm::vec4 v3 = transformPoint(t.v3, transformMatrix);

    glm::vec3 n = glm::cross(glm::vec3(v3 - v1), glm::vec3(v3 - v2));
    glm::vec4 c = (v1 + v2 + v3) / glm::vec4(3, 3, 3, 3);
    glm::vec3 np = glm::vec3(g_camera - c);
    if (glm::dot(n, np)/glm::distance(n, np) < 0) {
      continue;
    }

    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);

    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);

    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v1.x, v1.y, v1.z);
  }
  glEnd();

  glColor3f(0.0f, 1.0f, 1.0f);
  glPointSize(4);
  glBegin(GL_POINTS);
  for (auto p : g_polygon) {
    glm::vec4 point = transformPoint(p, transformMatrix);
    std::cout << point << std::endl;
    glVertex3f(point.x, point.y, point.z);
  }
  glEnd();

  glColor3f(1.0f, 0.0f, 1.0f);
  glLineWidth(3);
  glBegin(GL_LINE_STRIP);
  for (auto p : g_bezier_points) {
    glm::vec4 point = transformPoint(p, transformMatrix);
    glVertex3f(point.x, point.y, point.z);
  }
  glEnd();

  glm::vec4 O = transformPoint(glm::vec4(0, 0, 0, 1), transformMatrix);
  glm::vec4 X = transformPoint(glm::vec4(0.2, 0, 0, 1), transformMatrix);
  glm::vec4 Y = transformPoint(glm::vec4(0, 0.2, 0, 1), transformMatrix);
  glm::vec4 Z = transformPoint(glm::vec4(0, 0, 0.2, 1), transformMatrix);
  glLineWidth(2);
  glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(O.x, O.y, O.z); glVertex3f(X.x, X.y, X.z);
    glColor3f(0, 1, 0);
    glVertex3f(O.x, O.y, O.z); glVertex3f(Y.x, Y.y, Y.z);
    glColor3f(0, 0, 1);
    glVertex3f(O.x, O.y, O.z); glVertex3f(Z.x, Z.y, Z.z);
  glEnd();

  glFlush();
}

void resize(int width, int height) {
  g_width = width;
  g_height = height;
  glViewport(0, 0, g_width, g_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  gluOrtho2D(-g_width/2, g_width/2, -g_height/2, g_height/2);
}

int g_motion_start = 0, g_x = 0;
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    g_motion_start = 1;
    g_x = x;
  } else {
    g_motion_start = 0;
  }
}

void motion(int x, int y) {
  if (!g_motion_start) {
    return;
  }
  double alpha = (g_x - x) / glm::distance(g_camera, glm::vec4(0, 0, 0, 1));

  glm::mat4 Z = glm::transpose(glm::mat4(
     cos(alpha), sin(alpha), 0, 0,
    -sin(alpha), cos(alpha), 0, 0,
              0,          0, 1, 0,
              0,          0, 0, 1
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

  f = fopen(argv[2], "r");
  int n;
  float x, y, z;
  fscanf(f, "%d", &n);
  while (n--) {
    fscanf(f, "%f %f %f", &x, &y, &z);
    g_polygon.push_back(glm::vec4(x, y, z, 1));
  }
  fclose(f);

  g_bezier_points = calculateBezierPoints(g_polygon, 100);

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
