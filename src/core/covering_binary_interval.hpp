// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/is-majsoul-fair.

#if !defined(CORE_COVERING_BINARY_INTERVAL_HPP)
#define CORE_COVERING_BINARY_INTERVAL_HPP

#include "interval.hpp"
#include "integer.hpp"
#include <utility>
#include <cstddef>


namespace IsMajsoulFair{

std::pair<IsMajsoulFair::Integer, IsMajsoulFair::Integer> getCoveringBinaryInterval(
  IsMajsoulFair::Interval const &interval, std::size_t const num_bits);

} // namespace IsMajsoulFair

#endif // !defined(CORE_COVERING_BINARY_INTERVAL_HPP)
