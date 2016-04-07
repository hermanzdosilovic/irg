#include <stdio.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include "bresenham.h"

const int kMaxNumberOfVertices = 50;
const int kMaxNumberOfPolygons = 50;
const int kWidth = 500, kHeight = 500;

struct Polygon {
  glm::vec3 vertices[kMaxNumberOfVertices + 1];
  int number_of_vertices;
} g_polygons[kMaxNumberOfPolygons + 1];

int g_number_of_polygons;
glm::vec3 g_point;

std::ostream &operator<< (std::ostream &out, const glm::vec3 &v) {
  out << "(" << v.x << ", " << v.y << ")";
  return out;
}

int drawPolygon(Polygon polygon) {
  if (polygon.number_of_vertices == 0) {
    return -1;
  }
  
  glm::vec3 *vertices = polygon.vertices;
  for (int i = 0; i < polygon.number_of_vertices - 1; i++) {
    drawLine(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y);
  }

  drawLine(
    vertices[polygon.number_of_vertices - 1].x,
    vertices[polygon.number_of_vertices - 1].y,
    vertices[0].x,
    vertices[0].y
  );

  return 0;
}

void addPoint(int x, int y) {
  int *i = &g_polygons[g_number_of_polygons].number_of_vertices;
  if (*i + 1 > kMaxNumberOfVertices) {
    printf("Maximum number of vertices exceeded!\n");
    return;
  }

  g_polygons[g_number_of_polygons].vertices[*i] = glm::vec3(x, y, 1);
  *i += 1;
}

int polygonPointRelation(glm::vec3 point, Polygon polygon) {
  int N = polygon.number_of_vertices;
  glm::vec3 edges[polygon.number_of_vertices + 1];
  for (int i = 0; i < N; i++) { // expecting vertices in clockwise direction
    edges[i] = glm::cross(polygon.vertices[i], polygon.vertices[(i + 1)%N]);
  }

  int inside = 1;
  for (int i = 0; i < N; i++) {
    if (!(glm::dot(point, edges[i]) >= 0)) { // >= because of reverse coordinate system (y is upside down)
      inside = 0;
      break;
    }
  }
  
  return inside;
}

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    addPoint(x, y);
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    if (g_number_of_polygons + 1 > kMaxNumberOfVertices) {
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
    g_point = glm::vec3(x, y, 1);
    std::cout << "Point " << g_point << " relation with polygons:" << std::endl;
    for (int i = 0; i < g_number_of_polygons; i++) {
      int is_inside = polygonPointRelation(g_point, g_polygons[i]);
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

