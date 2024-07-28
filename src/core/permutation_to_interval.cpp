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
  IsMajsoulFair::Integer lower(0ul);
  IsMajsoulFair::Integer upper(1ul);

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
      lower *= denominator_factor;
      upper *= denominator_factor;

      lower += offset;
      upper += offset + num_tiles[tile];

      --num_tiles[tile];
    }
  }

  return Interval{denominator, lower, upper};
}

} // namespace IsMajsoulFair
