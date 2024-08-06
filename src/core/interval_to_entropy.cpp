// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/is-majsoul-fair.

#include "interval_to_entropy.hpp"

#include "covering_binary_interval.hpp"
#include "interval.hpp"
#include "integer.hpp"
#include "../common/throw.hpp"
#include <cmath>
#include <cstddef>


namespace IsMajsoulFair{

double intervalToEntropy(IsMajsoulFair::Interval const &interval, std::size_t const num_bits)
{
  auto const [lower_binary, upper_binary] = IsMajsoulFair::getCoveringBinaryInterval(interval, num_bits);

  IsMajsoulFair::Integer const binary_denominator = [&]() {
    IsMajsoulFair::Integer base(2ul);
    return base.inplacePow(num_bits);
  }();

  if (upper_binary - lower_binary == 1ul) {
    if (lower_binary * interval.getDenominator() > interval.getLowerNumerator() * binary_denominator) {
      IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
    }
    if (upper_binary * interval.getDenominator() < interval.getUpperNumerator() * binary_denominator) {
      IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
    }
    return 0.0;
  }

  IsMajsoulFair::Integer const normalizer = (upper_binary - lower_binary) * interval.getDenominator();

  double entropy = 0.0;
  {
    {
      IsMajsoulFair::Integer const probability_mass
        = (lower_binary + 1ul) * interval.getDenominator() - interval.getLowerNumerator() * binary_denominator;
      if (probability_mass < 0l) {
        IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
      }
      double const prob = IsMajsoulFair::divideAsDouble(probability_mass, normalizer);
      entropy -= prob * std::log2(prob);
    }
    for (IsMajsoulFair::Integer i = lower_binary + 1ul; i + 1ul < upper_binary; ++i) {
      IsMajsoulFair::Integer const probability_mass = interval.getDenominator();
      double const prob = IsMajsoulFair::divideAsDouble(probability_mass, normalizer);
      entropy -= prob * std::log2(prob);
    }
    {
      IsMajsoulFair::Integer const probability_mass
        = interval.getUpperNumerator() * binary_denominator - (upper_binary - 1ul) * interval.getDenominator();
      if (probability_mass < 0l) {
        IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
      }
      double const prob = IsMajsoulFair::divideAsDouble(probability_mass, normalizer);
      entropy -= prob * std::log2(prob);
    }
  }
  return entropy;
}

} // namespace IsMajsoulFair
