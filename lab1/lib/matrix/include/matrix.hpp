#pragma once

#include <vector>

namespace matrix {

struct Matrix {
  std::vector<std::vector<int>> rows;
  std::size_t rows_count;
  std::size_t columns_count;

  explicit Matrix(const std::size_t rows_count,
                  const std::size_t columns_count);
  explicit Matrix(const std::size_t size);

  void FillWithRandomNumbers();
  void Print() const;

  bool operator==(const Matrix &other) const;
};

Matrix MultiplyByRows(const Matrix &first, const Matrix &second);

Matrix MultiplyByColumns(const Matrix &first, const Matrix &second);

void MultiplyInChunk(Matrix &result, const Matrix &first, const Matrix &second,
                     const std::size_t row, const std::size_t column,
                     const std::size_t chunk_size);

} // namespace matrix
