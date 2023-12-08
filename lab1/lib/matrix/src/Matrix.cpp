#include "Matrix.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <numbers>
#include <random>

namespace matrix {

namespace {

const int kLowerBound = -128;
const int kUpperBound = 127;

void MultiplyByRowsChunk(
    Matrix& result, const Matrix& first, const Matrix& second,
    const std::size_t row_number, const std::size_t column_number,
    const std::size_t chunk_size) {
  const auto bottom_border = std::min(row_number + chunk_size,
                                      result.rows_count);
  const auto right_border = std::min(column_number + chunk_size,
                                     result.columns_count);

  for (auto i = row_number; i < bottom_border; i++) {
    for (auto j = column_number; j < right_border; j++) {
      for (auto k = 0; k < first.columns_count; k++) {
        result.rows[i][j] += first.rows[i][k] * second.rows[k][j];
      }
    }
  }
}

void MultiplyByColumnsChunk(
    Matrix& result, const Matrix& first, const Matrix& second,
    const std::size_t row_number, const std::size_t column_number,
    const std::size_t chunk_size) {
  const auto right_border = std::min(column_number + chunk_size,
                                     result.columns_count);
  const auto bottom_border = std::min(row_number + chunk_size,
                                      result.rows_count);

  for (auto i = column_number; i < right_border; i++) {
    for (auto j = row_number; j < bottom_border; j++) {
      for (auto k = 0; k < first.columns_count; k++) {
        result.rows[j][i] += first.rows[j][k] * second.rows[k][i];
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

bool Matrix::operator==(const Matrix &other) const {
  return rows_count == other.rows_count &&
         columns_count == other.columns_count &&
         rows == other.rows;
}

Matrix MultiplyWithTiming(const MultiplicationFunction& multiply,
    std::string_view type, const Matrix& first, const Matrix& second) {
  const auto start{std::chrono::steady_clock::now()};
  const auto result = multiply(first, second);
  const auto end{std::chrono::steady_clock::now()};

  const std::chrono::duration<double> elapsed_seconds{end - start};
  std::cout << "Multiplication " << type << " time spent: "
            << elapsed_seconds.count() << '\n';

  return result;
}

Matrix MultiplyByRows(const Matrix& first, const Matrix& second) {
  assert(first.columns_count == second.rows_count);

  Matrix result(first.rows_count, second.columns_count);
  MultiplyByRowsChunk(result, first, second, 0, 0, result.rows_count);
  return result;
}

Matrix MultiplyByColumns(const Matrix& first, const Matrix& second) {
  assert(first.columns_count == second.rows_count);

  Matrix result(first.rows_count, second.columns_count);
  MultiplyByColumnsChunk(result, first, second, 0, 0, result.rows_count);
  return result;
}

} // namespace matrix

