// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/is-majsoul-fair.

#if !defined(CORE_INTERVAL_TO_ENTROPY_HPP)
#define CORE_INTERVAL_TO_ENTROPY_HPP

#include "interval.hpp"
#include <cstddef>


namespace IsMajsoulFair{

double intervalToEntropy(IsMajsoulFair::Interval const &interval, std::size_t const num_bits);

} // namespace IsMajsoulFair

#endif // !defined(CORE_INTERVAL_TO_ENTROPY_HPP)
