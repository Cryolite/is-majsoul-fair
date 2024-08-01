// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/tsumonya

#include "interval_to_binary.hpp"

#include "../common/throw.hpp"
#include <random>
#include <vector>
#include <utility>
#include <stdexcept>
#include <climits>
#include <cstddef>


namespace IsMajsoulFair{

namespace{

std::pair<IsMajsoulFair::Integer, IsMajsoulFair::Integer> getCoveringBinaryInterval(
  IsMajsoulFair::Interval const &interval,
  std::size_t const num_bits)
{
  IsMajsoulFair::Integer const binary_denominator = [&]() {
    IsMajsoulFair::Integer term(2ul);
    return term.inplacePow(num_bits);
  }();

  IsMajsoulFair::Integer const original_lower_numerator = interval.getLowerNumerator() * binary_denominator;
  IsMajsoulFair::Integer binary_lower_numerator(0ul);
  IsMajsoulFair::Integer binary_lower_numerator_term = interval.getDenominator() * binary_denominator;
  for (std::size_t i = 0u; i < num_bits; ++i) {
    if (binary_lower_numerator + binary_lower_numerator_term <= original_lower_numerator) {
      binary_lower_numerator += binary_lower_numerator_term;
    }
    binary_lower_numerator_term /= 2ul;
  }
  if (binary_lower_numerator <= original_lower_numerator && original_lower_numerator < binary_lower_numerator + interval.getDenominator()) {
    // OK
  }
  else {
    IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
  }

  IsMajsoulFair::Integer const original_upper_numerator = interval.getUpperNumerator() * binary_denominator;
  IsMajsoulFair::Integer binary_upper_numerator(0ul);
  IsMajsoulFair::Integer binary_upper_numerator_term = interval.getDenominator() * binary_denominator;
  for (std::size_t i = 0u; i < num_bits; ++i) {
    if (binary_upper_numerator + binary_upper_numerator_term <= original_upper_numerator) {
      binary_upper_numerator += binary_upper_numerator_term;
    }
    binary_upper_numerator_term /= 2ul;
  }
  if (binary_upper_numerator < original_upper_numerator + interval.getDenominator() && original_upper_numerator <= binary_upper_numerator) {
    // OK
  }
  else {
    IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
  }

  return {binary_lower_numerator / interval.getDenominator(), binary_upper_numerator / interval.getDenominator()};
}

} // namespace <unnamed>

std::vector<unsigned char> intervalToBinary(
  Interval const &interval, std::size_t const num_bits, IsMajsoulFair::IntegerRandomState &state)
{
  auto const [lower_binary, upper_binary] = getCoveringBinaryInterval(interval, num_bits);
  if (upper_binary - lower_binary > IsMajsoulFair::Integer(SIZE_MAX)) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`num_bits` is too large.");
  }

  IsMajsoulFair::Integer const binary_denominator = [&]() {
    IsMajsoulFair::Integer base(2ul);
    return base.inplacePow(num_bits);
  }();
  std::vector<IsMajsoulFair::Integer> probability_masses;
  {
    IsMajsoulFair::Integer probability_mass
      = (lower_binary + 1ul) * interval.getDenominator() - interval.getLowerNumerator() * binary_denominator;
    probability_masses.push_back(std::move(probability_mass));
  }
  for (IsMajsoulFair::Integer i = lower_binary + 1ul; i + 1ul < upper_binary; ++i) {
    IsMajsoulFair::Integer probability_mass = interval.getDenominator();
    probability_masses.push_back(std::move(probability_mass));
  }
  {
    IsMajsoulFair::Integer probability_mass
      = interval.getUpperNumerator() * binary_denominator - (upper_binary - 1ul) * interval.getDenominator();
    probability_masses.push_back(std::move(probability_mass));
  }

  IsMajsoulFair::Integer result_as_integer = [&]() {
    Integer random_value;
    random_value.setToRandom(
      state,
      interval.getLowerNumerator() * binary_denominator,
      interval.getUpperNumerator() * binary_denominator);
    for (unsigned long i = 0u; i < probability_masses.size(); ++i) {
      if (random_value < probability_masses[i]) {
        return lower_binary + i;
      }
      random_value -= probability_masses[i];
    }
    IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
    return Integer(0ul);
  }();

  std::vector<unsigned char> result;
  for (std::size_t i = 0u; i < num_bits; ++i) {
    unsigned char const bit = static_cast<unsigned long>(result_as_integer % 2ul);
    result_as_integer /= 2ul;
    std::size_t const index = i / 8u;
    std::size_t const offset = 7u - i % 8u;
    if (result.size() <= index) {
      result.resize(index + 1u, 0u);
    }
    result[index] |= bit << offset;
  }

  return result;
}

} // namespace IsMajsoulFair
