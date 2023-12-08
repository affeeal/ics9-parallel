#pragma once

#include <functional>
#include <string_view>

#include "matrix.hpp"

namespace utils {

using MultiplicationFunction =
    std::function<matrix::Matrix(const matrix::Matrix&, const matrix::Matrix&)>;

matrix::Matrix MultiplyOrdinaryWithTiming(MultiplicationFunction multiply,
                                          std::string_view type,
                                          const matrix::Matrix& first,
                                          const matrix::Matrix& second);

matrix::Matrix MultiplyByChunksWithTiming(const matrix::Matrix& first,
                                          const matrix::Matrix& second,
                                          const std::size_t chunk_size);

}  // namespace utils
