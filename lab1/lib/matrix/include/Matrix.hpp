#pragma once

#include <functional>
#include <vector>
#include <string_view>

namespace matrix {

struct Matrix;

using MultiplicationFunction =
    std::function<Matrix(const Matrix&, const Matrix&)>;

struct Matrix {
  std::vector<std::vector<int>> rows;
  std::size_t rows_count;
  std::size_t columns_count;

  explicit Matrix() = default;
  explicit Matrix(const std::size_t rows_count,
                  const std::size_t columns_count);

  void FillWithRandomNumbers();
  void Print() const;

  bool operator==(const Matrix& other) const;
};

Matrix MultiplyWithTiming(const MultiplicationFunction& multiply,
    std::string_view type, const Matrix& first, const Matrix& second);

Matrix MultiplyByRows(const Matrix& first, const Matrix& second);

Matrix MultiplyByColumns(const Matrix& first, const Matrix& second);

} // namespace matrix

