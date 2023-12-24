#include <mpi.h>

#include <cassert>
#include <chrono>
#include <eigen3/Eigen/Eigen>

#include "utils.hpp"

int main() {
  MPI_Init(nullptr, nullptr);

  auto comm_size = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  auto comm_rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

  const auto slice_size = utils::kDimension / comm_size;
  const auto from = slice_size * comm_rank;
  const auto to = from + slice_size;

  const auto coeffs_slice = utils::GetTestCoeffsSlice(from, to);
  const auto free_coeffs = utils::GetTestFreeCoeffs();

  auto start{std::chrono::steady_clock::now()};
  const auto solution =
      utils::CalculateSolution(coeffs_slice, free_coeffs, from, to);
  auto end{std::chrono::steady_clock::now()};

  std::chrono::duration<double> elapsed_seconds{end - start};
  if (comm_rank == utils::kMainProcessRank) {
    std::cout << "Ordinary solution time spent: " << elapsed_seconds.count()
              << '\n';
  }

  const auto free_coeffs_slice = free_coeffs(Eigen::seq(from, to - 1));

  start = std::chrono::steady_clock::now();
  const auto solution_slice =
      utils::CalculateSolutionSlice(coeffs_slice, free_coeffs_slice, from, to);
  end = std::chrono::steady_clock::now();

  elapsed_seconds = end - start;
  if (comm_rank == utils::kMainProcessRank) {
    std::cout << "Sliced solution time spent: " << elapsed_seconds.count()
              << '\n';
  }

  Eigen::VectorXd assembled_solution(utils::kDimension);
  MPI_Allgather(solution_slice.data(), solution_slice.size(), MPI_DOUBLE,
                assembled_solution.data(), solution_slice.size(), MPI_DOUBLE,
                MPI_COMM_WORLD);

  assert(solution == assembled_solution);

  MPI_Finalize();

  return 0;
}
