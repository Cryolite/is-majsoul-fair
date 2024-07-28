#include "interval_to_binary.hpp"

#include "../common/throw.hpp"
#include <random>
#include <vector>
#include <stdexcept>
#include <cstddef>


namespace IsMajsoulFair{

namespace{

IsMajsoulFair::Integer calculateLowerBinaryNumerator(
  IsMajsoulFair::Integer const &denominator,
  IsMajsoulFair::Integer const &numerator,
  std::size_t const num_bits)
{
  IsMajsoulFair::Integer const binary_denominator = [&]() {
    IsMajsoulFair::Integer term(2ul);
    return term.inplacePow(num_bits);
  }();
  IsMajsoulFair::Integer const original_numerator(numerator * binary_denominator);

  IsMajsoulFair::Integer target_numerator(0ul);
  IsMajsoulFair::Integer target_numerator_term(original_numerator);
  for (std::size_t i = num_bits; i > 0u;) {
    --i;
    target_numerator_term /= 2ul;
    if (target_numerator + target_numerator_term < original_numerator) {
      target_numerator += target_numerator_term;
    }
  }

  if (target_numerator <= original_numerator && original_numerator < target_numerator + denominator) {
    // OK
  }
  else {
    IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
  }
  return target_numerator / numerator;
}

} // namespace <unnamed>

std::vector<unsigned char> intervalToBinary(Interval const &interval, std::size_t const num_bits, std::mt19937 &rng)
{
  std::vector<unsigned char> result(num_bits / 8u, 0u);

  if (num_bits == 0u) {
    return result;
  }

  if (interval.lower() == interval.upper()) {
    result[0] = 1;
    return result;
  }

  if (interval.lower() == 0 && interval.upper() == interval.denominator()) {
    result[num_bits - 1] = 1;
    return result;
  }

  auto const range = interval.upper() - interval.lower();
  auto const step = range / (num_bits - 2);
  auto const remainder = range % (num_bits - 2);

  auto const lower = interval.lower();
  auto const upper = interval.upper();

  for (std::size_t i = 1; i < num_bits - 1; ++i) {
    if (i <= remainder) {
      result[i] = 1;
    }
    else {
      auto const value = lower + step * (i - remainder);
      if (value <= upper) {
        result[i] = 1;
      }
    }
  }

  return result;
}

} // namespace IsMajsoulFair
