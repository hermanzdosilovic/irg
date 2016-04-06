#include <stdio.h>
#include <GL/freeglut.h>
#include "bresenham.h"

const int kMaxNumberOfVertices = 50;
const int kMaxNumberOfPolygons = 50;
const int kWidth = 500, kHeight = 500;

struct Point {
  int x, y;
} g_vertices[kMaxNumberOfPolygons + 1][kMaxNumberOfVertices + 1];

int g_number_of_vertices;
int g_number_of_polygons;

Point g_point;

void drawPolygon(Point *vertices, int number_of_vertices) {
  if (number_of_vertices == 0) {
    return;
  }

  for (int i = 0; i < number_of_vertices - 1; i++) {
    drawLine(vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y);
  }

  drawLine(
    vertices[number_of_vertices - 1].x,
    vertices[number_of_vertices - 1].y,
    vertices[0].x,
    vertices[0].y
  );
}

void addPoint(int x, int y) {
  if (g_number_of_vertices + 1 > kMaxNumberOfVertices) {
    printf("Maximum number of vertices exceeded!\n");
    return;
  }
  g_vertices[g_number_of_polygons][g_number_of_vertices++] = {x, y};
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
    drawPolygon(g_vertices[g_number_of_polygons++], g_number_of_vertices);

    g_number_of_vertices = 0;
  } else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
    g_point = {x, y};
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

