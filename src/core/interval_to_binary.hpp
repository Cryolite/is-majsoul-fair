// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/tsumonya

#if !defined(CORE_INTERVAL_TO_BINARY_HPP)
#define CORE_INTERVAL_TO_BINARY_HPP

#include "interval.hpp"
#include "integer.hpp"
#include <vector>
#include <cstddef>


namespace IsMajsoulFair{

std::vector<unsigned char> intervalToBinary(
  IsMajsoulFair::Interval const &interval,
  std::size_t num_bits,
  IsMajsoulFair::IntegerRandomState &state);

} // namespace IsMajsoulFair

#endif // !defined(CORE_INTERVAL_TO_BINARY_HPP)
