#include "utils.hpp"

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "matrix.hpp"

namespace utils {

matrix::Matrix MultiplyOrdinaryWithTiming(MultiplicationFunction multiply,
                                          std::string_view type,
                                          const matrix::Matrix &first,
                                          const matrix::Matrix &second) {
  const auto start{std::chrono::steady_clock::now()};
  const auto result = multiply(first, second);

  const auto end{std::chrono::steady_clock::now()};
  const std::chrono::duration<double> elapsed_seconds{end - start};
  std::cout << "Multiplication " << type << ", time spent "
            << elapsed_seconds.count() << '\n';

  return result;
}

matrix::Matrix MultiplyByChunksWithTiming(const matrix::Matrix &first,
                                          const matrix::Matrix &second,
                                          const std::size_t chunk_size) {
  matrix::Matrix result(first.rows_count, second.columns_count);

  const auto chunks_count =
      std::ceil(static_cast<double>(result.rows_count) / chunk_size) *
      std::ceil(static_cast<double>(result.columns_count) / chunk_size);
  std::vector<std::thread> threads;
  threads.reserve(chunks_count);

  const auto start{std::chrono::steady_clock::now()};
  for (auto row = 0; row < result.rows_count; row += chunk_size) {
    for (auto column = 0; column < result.columns_count; column += chunk_size) {
      threads.push_back(std::thread(matrix::MultiplyInChunk, std::ref(result),
                                    std::cref(first), std::cref(second), row,
                                    column, chunk_size));
    }
  }

  for (auto &thread : threads) {
    thread.join();
  }

  const auto end{std::chrono::steady_clock::now()};
  const std::chrono::duration<double> elapsed_seconds{end - start};
  std::cout << "Multiplication by chunks, time spent "
            << elapsed_seconds.count() << '\n';

  return result;
}

} // namespace utils
