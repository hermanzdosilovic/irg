#include <iostream>
#include <glm/mat3x3.hpp>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>

std::ostream &operator<< (std::ostream &out, const glm::vec3 &v) {
  out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return out;
}

std::ostream &operator<< (std::ostream &out, const glm::mat3x3 &m) {
  out << "{";
  for (int row = 0; row < 3; row++) {
    out << "(";
    for (int col = 0; col < 3; col++) {
      out << m[col][row];
      if (col + 1 != 3) out << ", ";
    }
    out << ")";
    if (row + 1 != 3) out << ", ";
  }
  out << "}";

  return out;
}

int main() {
  float Ax, Ay, Az;
  float Bx, By, Bz;
  float Cx, Cy, Cz;
  float Tx, Ty, Tz;
  
  scanf("%f %f %f", &Ax, &Ay, &Az);
  scanf("%f %f %f", &Bx, &By, &Bz);
  scanf("%f %f %f", &Cx, &Cy, &Cz);
  scanf("%f %f %f", &Tx, &Ty, &Tz);

  glm::mat3x3 M1 = glm::mat3x3( // column-major order
    Ax, Ay, Az,
    Bx, By, Bz,
    Cx, Cy, Cz
  );

  glm::vec3 V1 = glm::vec3(Tx, Ty, Tz);

  glm::vec3 V2 = glm::inverse(M1) * V1;
  
  std::cout << V2 << std::endl; 
  return 0;
}
