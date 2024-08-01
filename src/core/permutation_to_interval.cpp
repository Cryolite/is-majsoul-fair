// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/tsumonya

#include "permutation_to_interval.hpp"

#include "interval.hpp"
#include "integer.hpp"
#include "../common/throw.hpp"
#include <vector>
#include <array>
#include <cstdint>


namespace IsMajsoulFair{

Interval permutationToInterval(std::vector<std::uint_fast8_t> const &permutation)
{
  std::array<std::uint_fast8_t, 37u> num_tiles{
    1u, 4u, 4u, 4u, 4u, 3u, 4u, 4u, 4u, 4u,
    1u, 4u, 4u, 4u, 4u, 3u, 4u, 4u, 4u, 4u,
    1u, 4u, 4u, 4u, 4u, 3u, 4u, 4u, 4u, 4u,
    4u, 4u, 4u, 4u, 4u, 4u, 4u
  };

  IsMajsoulFair::Integer denominator(1ul);
  IsMajsoulFair::Integer lower_numerator(0ul);
  IsMajsoulFair::Integer upper_numerator(1ul);

  {
    unsigned long denominator_factor = 136ul;
    for (std::uint_fast8_t i = 0u; i < permutation.size(); ++i, --denominator_factor) {
      std::uint_fast8_t const tile = permutation[i];
      if (tile >= num_tiles.size()) {
        IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("An invalid `permutation` was passed.");
      }
      if (num_tiles[tile] == 0u) {
        IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("An invalid `permutation` was passed.");
      }
      std::uint_fast8_t offset = 0u;
      for (std::uint_fast8_t j = 0u; j < tile; ++j) {
        offset += num_tiles[j];
      }

      denominator *= denominator_factor;
      lower_numerator *= denominator_factor;
      upper_numerator *= denominator_factor;

      lower_numerator += offset;
      upper_numerator += offset + num_tiles[tile];

      --num_tiles[tile];
    }
  }

  return {denominator, lower_numerator, upper_numerator};
}

} // namespace IsMajsoulFair
