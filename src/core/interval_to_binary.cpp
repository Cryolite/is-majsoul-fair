// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/is-majsoul-fair.

#include "interval_to_binary.hpp"

#include "covering_binary_interval.hpp"
#include "interval.hpp"
#include "integer.hpp"
#include "../common/throw.hpp"
#include <random>
#include <algorithm>
#include <vector>
#include <utility>
#include <stdexcept>
#include <climits>
#include <cstddef>


namespace IsMajsoulFair{

namespace{

std::vector<unsigned char> integerToBinary(IsMajsoulFair::Integer const &integer, std::size_t const num_bits)
{
  std::vector<unsigned char> result;
  {
    IsMajsoulFair::Integer value = integer;
    for (std::size_t i = 0u; i < num_bits; ++i) {
      unsigned char const bit = static_cast<unsigned long>(value % 2ul);
      value /= 2ul;
      result.push_back(bit);
    }
  }
  std::reverse(result.begin(), result.end());
  return result;
}

} // namespace <unnamed>

std::vector<unsigned char> intervalToBinary(
  Interval const &interval, std::size_t const num_bits, IsMajsoulFair::IntegerRandomState &state)
{
  auto const [lower_binary, upper_binary] = IsMajsoulFair::getCoveringBinaryInterval(interval, num_bits);
  if (upper_binary - lower_binary > IsMajsoulFair::Integer(SIZE_MAX)) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`num_bits` is too large.");
  }

  if (upper_binary - lower_binary == 1ul) {
    return integerToBinary(lower_binary, num_bits);
  }

  IsMajsoulFair::Integer const binary_denominator = [&]() {
    IsMajsoulFair::Integer base(2ul);
    return base.inplacePow(num_bits);
  }();
  std::vector<IsMajsoulFair::Integer> probability_masses;
  {
    IsMajsoulFair::Integer probability_mass
      = (lower_binary + 1ul) * interval.getDenominator() - interval.getLowerNumerator() * binary_denominator;
    if (probability_mass < 0l) {
      IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
    }
    probability_masses.push_back(std::move(probability_mass));
  }
  for (IsMajsoulFair::Integer i = lower_binary + 1ul; i + 1ul < upper_binary; ++i) {
    IsMajsoulFair::Integer probability_mass = interval.getDenominator();
    probability_masses.push_back(std::move(probability_mass));
  }
  {
    IsMajsoulFair::Integer probability_mass
      = interval.getUpperNumerator() * binary_denominator - (upper_binary - 1ul) * interval.getDenominator();
    if (probability_mass < 0l) {
      IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
    }
    probability_masses.push_back(std::move(probability_mass));
  }
  if (probability_masses.size() != upper_binary - lower_binary) {
    IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
  }

  IsMajsoulFair::Integer result_as_integer = [&]() {
    IsMajsoulFair::Integer random_value;
    random_value.setToRandom(
      state, (interval.getUpperNumerator() - interval.getLowerNumerator()) * binary_denominator);
    for (unsigned long i = 0u; i < probability_masses.size(); ++i) {
      if (random_value < probability_masses[i]) {
        return lower_binary + i;
      }
      random_value -= probability_masses[i];
    }
    IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
    return Integer(0ul);
  }();

  return integerToBinary(result_as_integer, num_bits);
}

} // namespace IsMajsoulFair
