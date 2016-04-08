#include <iostream>
#include <cstdio>
#include <vector>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include "polygon.h"
#include "polyartist.h"

int g_width = 500, g_height = 500;

std::vector<Polygon> g_polygons;

std::ostream &operator<< (std::ostream &out, const glm::vec3 &v) {
  out << "(" << v.x << ", " << v.y << ")";
  return out;
}

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    if (g_polygons.size() == 0) {
      g_polygons.push_back(Polygon());
    }
    g_polygons.back().AddVertex(glm::vec3(x, y, 1));
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    printf("Drawing polygon P%d.\n", (int) g_polygons.size());
    if (g_polygons.back().NumberOfVertices() == 0) {
      printf("  Failed. Polygon has no vertices.\n");
      return;
    } 
    printf("  Success.\n");
    drawPolygon(g_polygons.back());
    fillPolygon1(g_polygons.back());
    g_polygons.push_back(Polygon());
    glutSwapBuffers();
  } else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
    glm::vec3 point = glm::vec3(x, y, 1);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(2);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glutSwapBuffers();
    std::cout << "Point " << point << " relation with polygons:" << std::endl;
    for (int i = 0; i < g_polygons.size() - 1; i++) {
      int is_inside = g_polygons[i].PointRelation(point);
      std::cout << (is_inside ? "  Inside" : "  Outside") << " of polygon P" << i << std::endl;
    }
  }
}

void display() {
  glViewport(0, 0, g_width, g_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, g_width, g_height, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  for (int i = 0; i < g_polygons.size(); i++) {
    drawPolygon(g_polygons[i]);
    fillPolygon1(g_polygons[i]);
  }

  glutSwapBuffers();
}

void resize(int width, int height) {
  g_width = width;
  g_height = height;
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(200, 200); 
  
  glutCreateWindow("Polygon");

  if (argc == 2) {
    FILE *f = fopen(argv[1], "r");
    int number_of_polygons;
    fscanf(f, "%d", &number_of_polygons);
    for (int i = 0; i < number_of_polygons; i++) {
      int number_of_vertices;
      fscanf(f, "%d", &number_of_vertices);
      Polygon polygon;
      while (number_of_vertices--) {
        int x, y;
        fscanf(f, "%d %d", &x, &y);
        polygon.AddVertex(glm::vec3(x, y, 1));
      }
      g_polygons.push_back(polygon);
    }
    g_polygons.push_back(Polygon());
  }

  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMainLoop();

  return 0;
}

