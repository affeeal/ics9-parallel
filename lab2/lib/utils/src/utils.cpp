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
  return (coeffs * solution - free_coeffs).norm() / free_coeffs.norm();
}

}  // namespace

Matrix GetTestCoeffsSlice(const int from, const int to) {
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
                                    const Matrix& coeffs_slice) {
  return coeffs_slice * random;
}

Eigen::VectorXd CalculateSolution(const Matrix& coeffs_slice,
                                  const Eigen::VectorXd& free_coeffs,
                                  const int from, const int to) {
  Eigen::VectorXd solution(kDimension);
  solution.setZero();

  Matrix coeffs(kDimension, kDimension);
  MPI_Allgather(coeffs_slice.data(), coeffs_slice.size(), MPI_DOUBLE,
                coeffs.data(), coeffs_slice.size(), MPI_DOUBLE, MPI_COMM_WORLD);

  const auto free_coeffs_slice = free_coeffs(Eigen::seq(from, to - 1));

  auto velocity = 0.1 / kDimension;

  auto error_rate = std::numeric_limits<double>::max();
  while (error_rate > kEpsilon) {
    const auto new_solution_slice = static_cast<Eigen::VectorXd>(
        solution(Eigen::seq(from, to - 1)) -
        velocity * (coeffs_slice * solution - free_coeffs_slice));

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

Eigen::VectorXd CalculateSolutionSlice(const Matrix& coeffs_slice,
                                       const Eigen::VectorXd& free_coeffs_slice,
                                       const int from, const int to) {
  Eigen::VectorXd solution_slice(to - from);
  solution_slice.setZero();

  Eigen::VectorXd solution(kDimension);
  solution.setZero();

  Matrix coeffs(kDimension, kDimension);
  MPI_Allgather(coeffs_slice.data(), coeffs_slice.size(), MPI_DOUBLE,
                coeffs.data(), coeffs_slice.size(), MPI_DOUBLE, MPI_COMM_WORLD);

  Eigen::VectorXd free_coeffs(kDimension);
  MPI_Allgather(free_coeffs_slice.data(), free_coeffs_slice.size(), MPI_DOUBLE,
                free_coeffs.data(), free_coeffs_slice.size(), MPI_DOUBLE,
                MPI_COMM_WORLD);

  auto velocity = 0.1 / kDimension;

  auto error_rate = std::numeric_limits<double>::max();
  while (error_rate > kEpsilon) {
    solution_slice -= velocity * (coeffs_slice * solution - free_coeffs_slice);
    MPI_Allgather(solution_slice.data(), solution_slice.size(), MPI_DOUBLE,
                  solution.data(), solution_slice.size(), MPI_DOUBLE,
                  MPI_COMM_WORLD);

    const auto new_error_rate = GetErrorRate(solution, coeffs, free_coeffs);
    if (new_error_rate > error_rate) {
      velocity *= -1;  // happens once at most
    }

    error_rate = new_error_rate;
  }

  return solution_slice;
}

}  // namespace utils
