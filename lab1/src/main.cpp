#include <iostream>

#include "Matrix.hpp"

int main() {
  std::size_t size = 0, chunks_count = 0;
  std::cin >> size >> chunks_count;

  matrix::Matrix first(size, size);
  first.FillWithRandomNumbers();
  first.Print();

  std::cout << std::endl;

  matrix::Matrix second(size, size);
  second.FillWithRandomNumbers();
  second.Print();

  std::cout << std::endl;

  const auto result = matrix::MatrixMultiplication(first, second);
  result.Print();

  return 0;
}

