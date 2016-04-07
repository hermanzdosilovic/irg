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
  glPointSize(1);
  glColor3f(0.0f, 0.0f, 1.0f);
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
    int L = -1, R = 1 << 30;
    for (int i = 0; i < N; i++) {
      glm::vec3 edge = polygon.GetEdge(i);
      glm::mat2x2 coeff = glm::mat2x2(line.x, edge.x, line.y, edge.y);
      glm::vec2 intersection = glm::inverse(coeff) * glm::vec2(-line.z, -edge.z);

      glm::vec3 v1 = polygon.GetVertex(i);
      glm::vec3 v2 = polygon.GetVertex((i + 1)%N);
  
      if (v1.y >= v2.y && intersection.x > L) {
        L = intersection.x;
      } else if (v1.y < v2.y && intersection.x < R) {
        R = intersection.x;
      }
    }
  
    glPointSize(1);
    if (L < R) {
      glColor3f(1.0f, 0.0f, 0.0f);
      drawLine(L, y, R, y);
    }
  }
  glFlush();
}
