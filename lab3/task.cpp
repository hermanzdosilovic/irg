#include <stdio.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include "bresenham.h"
#include "polygon.h"

const int kMaxNumberOfPolygons = 50;
const int kWidth = 500, kHeight = 500;

Polygon g_polygons[kMaxNumberOfPolygons + 1];
int g_number_of_polygons;

std::ostream &operator<< (std::ostream &out, const glm::vec3 &v) {
  out << "(" << v.x << ", " << v.y << ")";
  return out;
}

int drawPolygon(Polygon polygon) {
  int N = polygon.NumberOfVertices();
  if (N == 0) {
    return -1;
  }
  
  for (int i = 0; i < N; i++) {
    glm::vec3 f = polygon.GetVertex(i);
    glm::vec3 s = polygon.GetVertex((i + 1)%N);
    drawLine(f.x, f.y, s.x, s.y);
  }

  return 0;
}

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    g_polygons[g_number_of_polygons].AddVertex(glm::vec3(x, y, 1));
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    if (g_number_of_polygons + 1 > kMaxNumberOfPolygons) {
      printf("Maximum number of polygons exceeded!\n");
      return;
    }
    printf("Drawing polygon P%d.\n", g_number_of_polygons);
    int ret = drawPolygon(g_polygons[g_number_of_polygons]);
    if (ret == 0) {
      printf("  Success.\n");
      g_number_of_polygons++;
    } else {
      printf("  Failed. Polygon has no vertices.\n");
    }
  } else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
    glm::vec3 point = glm::vec3(x, y, 1);
    std::cout << "Point " << point << " relation with polygons:" << std::endl;
    for (int i = 0; i < g_number_of_polygons; i++) {
      int is_inside = g_polygons[i].PointRelation(point);
      std::cout << (is_inside ? "  Inside" : "  Outside") << " of polygon P" << i << std::endl;
    }
  }
}

void display() {
  glViewport(0, 0, kWidth, kHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, kWidth, kHeight, 0);
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
  
  glutCreateWindow("Polygon");
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutMainLoop();

  return 0;
}

