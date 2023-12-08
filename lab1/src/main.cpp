#include <cassert>
#include <iostream>

#include "matrix.hpp"
#include "utils.hpp"

int main() {
  std::size_t matrix_size = 0, chunk_size = 0;
  std::cin >> matrix_size >> chunk_size;

  matrix::Matrix first(matrix_size);
  first.FillWithRandomNumbers();

  matrix::Matrix second(matrix_size);
  second.FillWithRandomNumbers();

  const auto by_rows = utils::MultiplyOrdinaryWithTiming(
      matrix::MultiplyByRows, "by rows", first, second);

  const auto by_columns = utils::MultiplyOrdinaryWithTiming(
      matrix::MultiplyByColumns, "by columns", first, second);

  assert(by_rows == by_columns);

  const auto by_chunks =
      utils::MultiplyByChunksWithTiming(first, second, chunk_size);

  assert(by_chunks == by_rows);

  return 0;
}
