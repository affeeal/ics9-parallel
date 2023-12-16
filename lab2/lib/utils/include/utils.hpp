#pragma once

#include <eigen3/Eigen/Dense>

namespace utils {

using Matrix =
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

constexpr int kDimension = 5000;
constexpr double kEpsilon = 1e-5;

Matrix GetTestCoeffs(const int from, const int to);

Eigen::VectorXd GetTestFreeCoeffs();

Eigen::VectorXd GetRandomVector();
Eigen::VectorXd GetRandomFreeCoeffs(const Eigen::VectorXd& random,
                                    const Matrix& coeffs);

Eigen::VectorXd CaclculateSolution(const Matrix& coeffs,
                                   const Eigen::VectorXd& free_coeffs,
                                   const int from, const int to);

}  // namespace utils
