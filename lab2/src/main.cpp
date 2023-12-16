#include <mpi.h>

#include "utils.hpp"

namespace {

constexpr int kMainProcessRank = 0;

}  // namespace

int main() {
  MPI_Init(nullptr, nullptr);

  auto size = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  auto rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const auto chunk_size = utils::kDimension / size;
  const auto from = chunk_size * rank;
  const auto to = from + chunk_size;

  const auto coefficients = utils::GetCoefficients(from, to);

  auto random = (rank == kMainProcessRank ? utils::GenerateRandom()
                                          : Eigen::VectorXd(utils::kDimension));
  MPI_Bcast(random.data(), utils::kDimension, MPI_DOUBLE, kMainProcessRank,
            MPI_COMM_WORLD);

  const auto free_coefficients =
      utils::GetFreeCoefficientsRandom(random, coefficients);

  // TODO

  MPI_Finalize();

  return 0;
}
