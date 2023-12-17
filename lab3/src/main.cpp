#include <chrono>
#include <iostream>

#include "utils.hpp"

int main() {
  const auto coeffs = utils::GetTestCoeffs();
  const auto free_coeffs = utils::GetTestFreeCoeffs();

  const auto start{std::chrono::steady_clock::now()};
  const auto solution = utils::CalculateSolution(coeffs, free_coeffs);
  const auto end{std::chrono::steady_clock::now()};

  const std::chrono::duration<double> elapsed_seconds{end - start};
  std::cout << "solution:\n" << solution << '\n';
  std::cout << "threads count: " << omp_get_max_threads() << '\n';
  std::cout << "time spent: " << elapsed_seconds.count() << '\n';
}
