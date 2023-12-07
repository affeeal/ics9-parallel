#include "Matrix.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <random>

// Uncomment to disable assertions
// #define NDEBUG

namespace matrix {

namespace {

const int kLowerBound = -128;
const int kUpperBound = 127;

void MatrixChunkMultiplication(
    Matrix& result, const Matrix& first, const Matrix& second,
    const std::size_t column_number, const std::size_t row_number,
    const std::size_t chunk_size) {
  assert(result.rows_count == first.rows_count);
  assert(result.columns_count == second.columns_count);
  assert(first.columns_count == second.rows_count);

  const auto right_border = std::min(column_number + chunk_size,
                                     result.columns_count);
  const auto bottom_border = std::min(row_number + chunk_size,
                                      result.rows_count);

  for (auto i = row_number; i < bottom_border; i++) {
    for (auto j = column_number; j < right_border; j++) {
      for (auto k = 0; k < first.columns_count; k++) {
        result.rows[i][j] += first.rows[i][k] * second.rows[k][j];
      }
    }
  }
}

} // namespace

Matrix::Matrix(const std::size_t rows_count, const std::size_t columns_count)
    : rows_count(rows_count), columns_count(columns_count) {
  rows.resize(rows_count);
  for (auto& row : rows) {
    row.resize(columns_count);
  }
}

void Matrix::FillWithRandomNumbers() {
  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_int_distribution<int> distribution(kLowerBound, kUpperBound);

  for (auto& row : rows) {
    for (auto& value : row) {
      value = distribution(generator);
    }
  }
}

void Matrix::Print() const {
  for (const auto& row : rows) {
    for (const auto value : row) {
      std::cout << value << ' '; 
    }

    std::cout << std::endl;
  }
}

Matrix MatrixMultiplication(const Matrix& first, const Matrix& second) {
  Matrix result(first.rows_count, second.columns_count);

  MatrixChunkMultiplication(result, first, second, 0, 0, result.rows_count);

  return result;
}

} // namespace matrix

