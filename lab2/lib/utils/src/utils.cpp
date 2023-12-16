#include "utils.hpp"

#include <mpi.h>

#include <limits>
#include <random>

namespace utils {

namespace {

std::mt19937 generator(std::random_device{}());
std::uniform_real_distribution<double> distribution(-1.0, 1.0);

double GetErrorRate(const Eigen::VectorXd& solution, const Matrix& coeffs,
                    const Eigen::VectorXd& free_coeffs) {
  Matrix all_coeffs(kDimension, kDimension);
  MPI_Allgather(coeffs.data(), coeffs.size(), MPI_DOUBLE, all_coeffs.data(),
                coeffs.size(), MPI_DOUBLE, MPI_COMM_WORLD);

  return (all_coeffs * solution - free_coeffs).norm() / free_coeffs.norm();
}

}  // namespace

Matrix GetTestCoeffs(const int from, const int to) {
  Matrix coeffs(to - from, kDimension);

  for (auto i = from; i < to; i++) {
    for (auto j = 0; j < kDimension; j++) {
      coeffs(i - from, j) = (i == j ? 2.0 : 1.0);
    }
  }

  return coeffs;
}

Eigen::VectorXd GetTestFreeCoeffs() {
  Eigen::VectorXd free_coeffs(kDimension);

  free_coeffs.setConstant(kDimension + 1);

  return free_coeffs;
}

Eigen::VectorXd GetRandomVector() {
  Eigen::VectorXd random(kDimension);

  for (auto& value : random) {
    value = distribution(generator);
  }

  return random;
}

Eigen::VectorXd GetRandomFreeCoeffs(const Eigen::VectorXd& random,
                                    const Matrix& coeffs) {
  return coeffs * random;
}

Eigen::VectorXd CaclculateSolution(const Matrix& coeffs,
                                   const Eigen::VectorXd& free_coeffs,
                                   const int from, const int to) {
  Eigen::VectorXd solution(kDimension);
  solution.setZero();

  auto velocity = 0.1 / kDimension;

  auto error_rate = std::numeric_limits<double>::max();
  while (error_rate > kEpsilon) {
    const Eigen::VectorXd new_solution_slice =
        solution(Eigen::seq(from, to - 1)) -
        velocity * (coeffs * solution - free_coeffs(Eigen::seq(from, to - 1)));

    MPI_Allgather(new_solution_slice.data(), new_solution_slice.size(),
                  MPI_DOUBLE, solution.data(), new_solution_slice.size(),
                  MPI_DOUBLE, MPI_COMM_WORLD);

    const auto new_error_rate = GetErrorRate(solution, coeffs, free_coeffs);
    if (new_error_rate > error_rate) {
      velocity *= -1;  // happens once at most
    }

    error_rate = new_error_rate;
  }

  return solution;
}

}  // namespace utils
