#include "polygon.h"

void Polygon::AddVertex(glm::vec3 vertex) {
  vertices_.push_back(vertex);
}

glm::vec3 Polygon::GetVertex(int index) {
  return vertices_[index];
}

glm::vec3 Polygon::GetEdge(int index) {
  int N = vertices_.size();
  return glm::cross(vertices_[index%N], vertices_[(index + 1)%N]);
}

int Polygon::NumberOfVertices() {
  return vertices_.size();
}

int Polygon::PointRelation(glm::vec3 point) {
  int inside = 1;
  for (int i = 0; i < this->NumberOfVertices(); i++) { // expecting vertices in clockwise direction
    if (!(glm::dot(point, this->GetEdge(i)) >= 0)) { // >= because of reverse coordinate system (y is upside down)
      inside = 0;
      break;
    }
  }
  
  return inside;
}

