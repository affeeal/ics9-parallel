#include "utils.hpp"

#include <omp.h>

#include <iostream>
#include <limits>

namespace utils {

namespace {

double CalculateError(const Eigen::VectorXd &solution, const Matrix &coeffs,
                      const Eigen::VectorXd &free_coeffs) {
  Eigen::VectorXd approximation(kDimension);

#pragma omp parallel for
  for (auto i = 0; i < kDimension; i++) {
    approximation[i] = coeffs(i, Eigen::all).dot(solution) - free_coeffs[i];
  }

  return approximation.norm() / free_coeffs.norm();
}

} // namespace

Matrix GetTestCoeffs() {
  Matrix coeffs(kDimension, kDimension);

  for (auto i = 0; i < kDimension; i++) {
    for (auto j = 0; j < kDimension; j++) {
      coeffs(i, j) = (i == j ? 2.0 : 1.0);
    }
  }

  return coeffs;
}

Eigen::VectorXd GetTestFreeCoeffs() {
  Eigen::VectorXd free_coeffs(kDimension);

  free_coeffs.setConstant(kDimension + 1);

  return free_coeffs;
}

Eigen::VectorXd CalculateSolution(const Matrix &coeffs,
                                  const Eigen::VectorXd &free_coeffs) {
  Eigen::VectorXd solution(kDimension);
  solution.setZero();

  auto velocity = 0.1 / kDimension;

  auto new_error = std::numeric_limits<double>::max();
  for (auto error = new_error; error > kEpsilon; error = new_error) {
    Eigen::VectorXd buffer(kDimension);

#pragma omp parallel for
    for (auto i = 0; i < kDimension; i++) {
      buffer[i] =
          solution[i] -
          velocity * (coeffs(i, Eigen::all).dot(solution) - free_coeffs[i]);
    }

    solution = std::move(buffer);

    new_error = CalculateError(solution, coeffs, free_coeffs);
    if (new_error > error) {
      velocity *= -1; // happens once at most
    }
  }

  return solution;
}

} // namespace utils
