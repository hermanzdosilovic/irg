#ifndef POLYGON_H
#define POLYGON_H
#include <vector>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

class Polygon {
  std::vector<glm::vec3> vertices_;

  public:
    void AddVertex(glm::vec3 vertex);
    glm::vec3 GetVertex(int index);
    glm::vec3 GetEdge(int index);
    int PointRelation(glm::vec3 point);
    int NumberOfVertices();
};

#endif
