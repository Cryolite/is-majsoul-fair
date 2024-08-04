// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/is-majsoul-fair.

#include "covering_binary_interval.hpp"

#include "interval.hpp"
#include "integer.hpp"
#include "../common/throw.hpp"
#include <utility>
#include <stdexcept>
#include <cstddef>


namespace IsMajsoulFair{

std::pair<IsMajsoulFair::Integer, IsMajsoulFair::Integer> getCoveringBinaryInterval(
  IsMajsoulFair::Interval const &interval, std::size_t const num_bits)
{
  IsMajsoulFair::Integer const binary_denominator = [&]() {
    IsMajsoulFair::Integer term(2ul);
    return term.inplacePow(num_bits);
  }();

  IsMajsoulFair::Integer const original_lower_numerator = interval.getLowerNumerator() * binary_denominator;
  IsMajsoulFair::Integer binary_lower_numerator(0ul);
  IsMajsoulFair::Integer binary_lower_numerator_term = interval.getDenominator() * binary_denominator;
  for (std::size_t i = 0u; i < num_bits; ++i) {
    binary_lower_numerator_term /= 2ul;
    if (binary_lower_numerator + binary_lower_numerator_term <= original_lower_numerator) {
      binary_lower_numerator += binary_lower_numerator_term;
    }
  }
  if (binary_lower_numerator <= original_lower_numerator && original_lower_numerator < binary_lower_numerator + interval.getDenominator()) {
    // OK
  }
  else {
    IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
  }

  IsMajsoulFair::Integer const original_upper_numerator = interval.getUpperNumerator() * binary_denominator;
  IsMajsoulFair::Integer binary_upper_numerator = interval.getDenominator() * binary_denominator;
  IsMajsoulFair::Integer binary_upper_numerator_term = interval.getDenominator() * binary_denominator;
  for (std::size_t i = 0u; i < num_bits; ++i) {
    binary_upper_numerator_term /= 2ul;
    if (binary_upper_numerator - binary_upper_numerator_term >= original_upper_numerator) {
      binary_upper_numerator -= binary_upper_numerator_term;
    }
  }
  if (binary_upper_numerator < original_upper_numerator + interval.getDenominator() && original_upper_numerator <= binary_upper_numerator) {
    // OK
  }
  else {
    if (binary_upper_numerator - interval.getDenominator() >= original_upper_numerator) {
      IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
    }
    IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
  }

  return {binary_lower_numerator / interval.getDenominator(), binary_upper_numerator / interval.getDenominator()};
}

} // namespace IsMajsoulFair
