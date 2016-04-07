#include "polyartist.h"
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat2x2.hpp>
#include <glm/matrix.hpp>
#include <glm/geometric.hpp>
#include "bresenham.h"
#include <algorithm>
#include <iostream>

void drawPolygon(Polygon polygon) {
  int N = polygon.NumberOfVertices();
  
  glm::vec3 first, second;
  for (int i = 0; i < N; i++) {
    first = polygon.GetVertex(i);
    second = polygon.GetVertex((i + 1)%N);
    drawLine(first.x, first.y, second.x, second.y);
  }
}

void fillPolygon1(Polygon polygon) {
  int N = polygon.NumberOfVertices();

  int minY = 1 << 30;
  int maxY = -1;
  for (int i = 0; i < N; i++) {
    glm::vec3 v = polygon.GetVertex(i);
    if (v.y > maxY) {
      maxY = v.y;
    }
    if (v.y < minY) {
      minY = v.y;
    }
  }
  
  for (int y = minY; y <= maxY; y++) {
    glm::vec3 line = glm::cross(glm::vec3(0, y, 1), glm::vec3(1, y, 1));
    int x[N + 1], intersection_count = 0;
    for (int i = 0; i < N; i++) {
      glm::vec3 edge = polygon.GetEdge(i);
      glm::mat2x2 coeff = glm::mat2x2(line.x, edge.x, line.y, edge.y);
      glm::vec2 intersection = glm::inverse(coeff) * glm::vec2(-line.z, -edge.z);

      glm::vec3 v1 = polygon.GetVertex(i);
      glm::vec3 v2 = polygon.GetVertex((i + 1)%N);
  
      int ix = intersection.x;
      if (ix >= v1.x && ix <= v2.x) {
        x[intersection_count++] = ix;
      } else if (ix >= v2.x && ix <= v1.x) {
        x[intersection_count++] = ix;
      }
    }

    if (intersection_count >= 2) {
      std::sort(x, x + intersection_count);
      glPointSize(1);
      glColor3f(1.0f, 0.0f, 0.0f);
      glBegin(GL_POINTS);
      for (int xi = x[0]; xi <= x[intersection_count - 1]; xi++) {
        glVertex2i(xi, y);
      }
      glEnd();
    }
  }
  glFlush();
}
