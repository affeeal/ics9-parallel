#include <cassert>
#include <iostream>

// Uncomment to disable assertions:
// #define NDEBUG

#include "Matrix.hpp"

int main() {
  std::size_t size = 0;
  std::cin >> size;

  matrix::Matrix first(size, size);
  first.FillWithRandomNumbers();

  matrix::Matrix second(size, size);
  second.FillWithRandomNumbers();

  const auto by_rows = matrix::MultiplyWithTiming(
      matrix::MultiplyByRows, "by rows", first, second);
  const auto by_columns = matrix::MultiplyWithTiming(
      matrix::MultiplyByColumns, "by columns", first, second);
  assert(by_rows == by_columns);

  return 0;
}

