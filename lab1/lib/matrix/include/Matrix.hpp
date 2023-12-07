#pragma once

#include <vector>

namespace matrix {

struct Matrix {
  std::vector<std::vector<int>> rows;
  std::size_t rows_count;
  std::size_t columns_count;

  explicit Matrix(const std::size_t rows_count,
                  const std::size_t columns_count);

  void FillWithRandomNumbers();
  void Print() const;
};

Matrix MatrixMultiplication(const Matrix& first, const Matrix& second);

} // namespace matrix

