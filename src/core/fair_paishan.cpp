#include "fair_paishan.hpp"

#include "../common/throw.hpp"
#include <random>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <cstdint>


namespace IsMajsoulFair{

std::vector<std::uint_fast8_t> generateFairPaishan(
  std::mt19937 &random_number_engine,
  std::uint_fast8_t const num_tiles)
{
  std::vector<std::uint_fast8_t> paishan(136u, 0u);
  std::iota(paishan.begin(), paishan.end(), 0u);
  std::shuffle(paishan.begin(), paishan.end(), random_number_engine);

  for (std::uint_fast8_t &tile : paishan) {
    if (0u <= tile && tile < 16u) {
      // 1m, 2m, 3m, 4m
      tile /= 4u;
      tile += 1u;
      continue;
    }
    if (tile == 16u) {
      // 0m
      tile = 0u;
      continue;
    }
    if (16u < tile && tile < 36u) {
      // 5m, 6m, 7m, 8m, 9m
      tile /= 4u;
      tile += 1u;
      continue;
    }
    if (36u <= tile && tile < 52u) {
      // 1p, 2p, 3p, 4p
      tile /= 4u;
      tile += 2u;
      continue;
    }
    if (tile == 52u) {
      // 0p
      tile = 10u;
      continue;
    }
    if (52u < tile && tile < 72u) {
      // 5p, 6p, 7p, 8p, 9p
      tile /= 4u;
      tile += 2u;
      continue;
    }
    if (72u <= tile && tile < 88u) {
      // 1s, 2s, 3s, 4s
      tile /= 4u;
      tile += 3u;
      continue;
    }
    if (tile == 88u) {
      // 0s
      tile = 20u;
      continue;
    }
    if (88u < tile) {
      // 5s, 6s, 7s, 8s, 9s, 1z, 2z, 3z, 4z, 5z, 6z, 7z
      tile /= 4u;
      tile += 3u;
      continue;
    }
    IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
  }

  paishan.resize(num_tiles);
  return paishan;
}

} // namespace IsMajsoulFair
