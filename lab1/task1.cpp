/*
g++ -o lab1 lab1.cpp -Iinclude
*/

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>
#include <glm/matrix.hpp>
#include <iostream>

std::ostream &operator<< (std::ostream &out, const glm::vec3 &v) {
  out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return out;
}

std::ostream &operator<< (std::ostream &out, const glm::mat3x3 &m) {
  out << "{" << "(" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ")"
      << ", " << "(" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ")"
      << ", " << "(" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ")" << "}";
  return out;
}

int main() {
  glm::vec3 v1 = glm::vec3(2, 3, -4) + glm::vec3(-1, 4, -1);
  std::cout << "v1 = " << v1 << std::endl;
  
  float s = glm::dot(v1, glm::vec3(-1, 4, -1));
  std::cout << "s = " << s << std::endl;
  
  glm::vec3 v2 = glm::cross(v1, glm::vec3(2, 2, 4));
  std::cout << "v2 = " << v2 << std::endl;

  glm::vec3 v3 = glm::normalize(v2);
  std::cout << "v3 = " << v3 << std::endl;

  glm::vec3 v4 = -1.0f * v2;
  std::cout << "v4 = " << v4 << std::endl;

  glm::mat3x3 A = glm::mat3x3(1, 2, 3, 2, 1, 3, 4, 5, 1);
  glm::mat3x3 B = glm::mat3x3(-1, 2, -3, 5, -2, 7, -4, -1, 3);
  glm::mat3x3 M1 = A + B;
  std::cout << "M1 = " << M1 << std::endl;

  glm::mat3x3 M2 = A * glm::transpose(B);
  std::cout << "M2 = " << M2 << std::endl;
  
  glm::mat3x3 M3 = A * glm::inverse(B);
  std::cout << "M3 = " << M3 << std::endl; 

  return 0;
}
