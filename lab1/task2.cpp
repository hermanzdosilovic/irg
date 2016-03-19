#include <iostream>
#include <glm/mat3x3.hpp>
#include <glm/matrix.hpp>

int main() {
  int coeff[12];
  for (int i = 0; i < 12; i++) {
    std::cin >> coeff[i];
  }
  
  glm::mat3x3 D = glm::mat3x3(
    coeff[0], coeff[1], coeff[2],
    coeff[4], coeff[5], coeff[6],
    coeff[8], coeff[9], coeff[10]
  );

  glm::mat3x3 Dx = glm::mat3x3(
    coeff[3], coeff[1], coeff[2],
    coeff[7], coeff[5], coeff[6],
    coeff[11], coeff[9], coeff[10]
  );

  glm::mat3x3 Dy = glm::mat3x3(
    coeff[0], coeff[3], coeff[2],
    coeff[4], coeff[7], coeff[6],
    coeff[8], coeff[11], coeff[10]
  );

  glm::mat3x3 Dz = glm::mat3x3(
    coeff[0], coeff[1], coeff[3],
    coeff[4], coeff[5], coeff[7],
    coeff[8], coeff[9], coeff[11]
  );
  
  float detD = glm::determinant(D);
  float x = glm::determinant(Dx)/detD;
  float y = glm::determinant(Dy)/detD;
  float z = glm::determinant(Dz)/detD;

  std::cout << "[x y z] = [" << x << " " << y << " " << z << "]" << std::endl;
  
  return 0;
}
