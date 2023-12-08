#include "matrix.hpp"

#include <iostream>
#include <random>

namespace matrix {

namespace {

const int kLowerBound = -128;
const int kUpperBound = 127;

}  // namespace

Matrix::Matrix(const std::size_t rows_count, const std::size_t columns_count)
    : rows_count(rows_count), columns_count(columns_count) {
  rows.resize(rows_count);
  for (auto &row : rows) {
    row.resize(columns_count);
  }
}

Matrix::Matrix(const std::size_t size) : Matrix(size, size) {}

void Matrix::FillWithRandomNumbers() {
  std::random_device device;
  std::mt19937 generator(device());
  std::uniform_int_distribution<int> distribution(kLowerBound, kUpperBound);

  for (auto &row : rows) {
    for (auto &value : row) {
      value = distribution(generator);
    }
  }
}

void Matrix::Print() const {
  for (const auto &row : rows) {
    for (const auto value : row) {
      std::cout << value << ' ';
    }

    std::cout << '\n';
  }
}

bool Matrix::operator==(const Matrix &other) const {
  return rows_count == other.rows_count &&
         columns_count == other.columns_count && rows == other.rows;
}

Matrix MultiplyByRows(const Matrix &first, const Matrix &second) {
  Matrix result(first.rows_count, second.columns_count);
  MultiplyInChunk(result, first, second, 0, 0, result.rows_count);
  return result;
}

Matrix MultiplyByColumns(const Matrix &first, const Matrix &second) {
  Matrix result(first.rows_count, second.columns_count);

  for (auto i = 0; i < result.columns_count; i++) {
    for (auto j = 0; j < result.rows_count; j++) {
      for (auto k = 0; k < first.columns_count; k++) {
        result.rows[j][i] += first.rows[j][k] * second.rows[k][i];
      }
    }
  }

  return result;
}

void MultiplyInChunk(Matrix &result, const Matrix &first, const Matrix &second,
                     const std::size_t row, const std::size_t column,
                     const std::size_t chunk_size) {
  const auto bottom_border = std::min(row + chunk_size, result.rows_count);
  const auto right_border = std::min(column + chunk_size, result.columns_count);

  for (auto i = row; i < bottom_border; i++) {
    for (auto j = column; j < right_border; j++) {
      for (auto k = 0; k < first.columns_count; k++) {
        result.rows[i][j] += first.rows[i][k] * second.rows[k][j];
      }
    }
  }
}

}  // namespace matrix
