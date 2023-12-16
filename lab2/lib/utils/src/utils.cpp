#include "utils.hpp"

#include <random>

namespace utils {

namespace {

std::mt19937 generator(std::random_device{}());
std::uniform_real_distribution<double> distribution(-1.0, 1.0);

}  // namespace

MatrixXd GetCoefficients(const int from, const int to) {
  MatrixXd coefficients(to - from, kDimension);

  for (auto i = from; i < to; i++) {
    for (auto j = 0; j < kDimension; j++) {
      coefficients(i - from, j) = (i == j ? 2.0 : 1.0);
    }
  }

  return coefficients;
}

VectorXd GetFreeCoefficientsSpecified() {
  VectorXd free_coefficients(kDimension);

  free_coefficients.setConstant(kDimension + 1);

  return free_coefficients;
}

VectorXd GenerateRandom() {
  VectorXd random(kDimension);

  for (auto& value : random) {
    value = distribution(generator);
  }

  return random;
}

VectorXd GetFreeCoefficientsRandom(const VectorXd& random,
                                   const MatrixXd& coefficients) {
  return coefficients * random;
}

}  // namespace utils
