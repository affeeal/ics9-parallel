#pragma once

#include <eigen3/Eigen/Dense>

namespace utils {

using Eigen::MatrixXd, Eigen::VectorXd;

constexpr int kDimension = 8;
constexpr double kEpsilon = 1e-5;

MatrixXd GetCoefficients(const int from, const int to);

VectorXd GetFreeCoefficientsSpecified();

VectorXd GenerateRandom();
VectorXd GetFreeCoefficientsRandom(const VectorXd& random,
                                   const MatrixXd& coefficients);

}  // namespace utils
