#pragma once

#include <eigen3/Eigen/Dense>

namespace utils {

using Matrix =
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

constexpr int kDimension = 20000;
constexpr double kEpsilon = 1e-5;

Matrix GetTestCoeffs();
Eigen::VectorXd GetTestFreeCoeffs();

Eigen::VectorXd CalculateSolution(const Matrix &coeffs,
                                  const Eigen::VectorXd &free_coeffs);

} // namespace utils
