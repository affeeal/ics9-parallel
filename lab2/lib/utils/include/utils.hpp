#pragma once

#include <eigen3/Eigen/Dense>

namespace utils {

using Matrix =
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

constexpr int kMainProcessRank = 0;

constexpr int kDimension = 20000;
constexpr double kEpsilon = 1e-5;

Matrix GetTestCoeffsSlice(const int from, const int to);

Eigen::VectorXd GetTestFreeCoeffs();

Eigen::VectorXd CalculateSolution(const Matrix &coeffs_slice,
                                  const Eigen::VectorXd &free_coeffs,
                                  const int from, const int to);

Eigen::VectorXd CalculateSolutionSlice(const Matrix &coeffs_slice,
                                       const Eigen::VectorXd &free_coeffs_slice,
                                       const int from, const int to);

} // namespace utils
