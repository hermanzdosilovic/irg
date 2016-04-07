#include <stdio.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include "polygon.h"
#include "polyartist.h"

const int kMaxNumberOfPolygons = 50;
const int kWidth = 500, kHeight = 500;

Polygon g_polygons[kMaxNumberOfPolygons + 1];
int g_number_of_polygons;

std::ostream &operator<< (std::ostream &out, const glm::vec3 &v) {
  out << "(" << v.x << ", " << v.y << ")";
  return out;
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
    if (g_polygons[g_number_of_polygons].NumberOfVertices() == 0) {
      printf("  Failed. Polygon has no vertices.\n");
      return;
    } 
    printf("  Success.\n");
    drawPolygon(g_polygons[g_number_of_polygons]);
    fillPolygon1(g_polygons[g_number_of_polygons++]);
  } else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
    glm::vec3 point = glm::vec3(x, y, 1);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
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

