#include <mpi.h>

#include "utils.hpp"

static constexpr int kMainProcessRank = 0;

int main() {
  MPI_Init(nullptr, nullptr);

  auto size = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  auto rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const auto chunk_size = utils::kDimension / size;
  const auto from = chunk_size * rank;
  const auto to = from + chunk_size;

  const auto test_coeffs = utils::GetTestCoeffs(from, to);
  const auto test_free_coeffs = utils::GetTestFreeCoeffs();

  const auto solution =
      utils::CaclculateSolution(test_coeffs, test_free_coeffs, from, to);

  // TODO

  MPI_Finalize();

  return 0;
}
