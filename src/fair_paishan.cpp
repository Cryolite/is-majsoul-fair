// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/is-majsoul-fair.

#include "common/throw.hpp"
#include <boost/lexical_cast.hpp>
#include <random>
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstddef>


namespace{

std::mt19937 createRandomNumberEngine()
{
  std::random_device random_device;
  std::array<std::mt19937::result_type, std::mt19937::state_size> seed_data;
  std::generate(seed_data.begin(), seed_data.end(), std::ref(random_device));
  std::seed_seq seed_sequence(seed_data.begin(), seed_data.end());
  return std::mt19937(seed_sequence);
}

std::vector<std::uint_fast8_t> createPaishan(std::size_t const num_tiles, std::mt19937 &random_number_engine)
{
  std::vector<std::uint_fast8_t> paishan(136u, 0u);
  std::iota(paishan.begin(), paishan.end(), 0u);
  std::shuffle(paishan.begin(), paishan.end(), random_number_engine);
  for (std::uint_fast8_t &tile : paishan) {
    if (0u <= tile && tile < 16u) {
      tile /= 4u;
      tile += 1u;
      continue;
    }
    if (tile == 16u) {
      tile = 0u;
      continue;
    }
    if (16u < tile && tile < 36u) {
      tile /= 4u;
      tile += 1u;
      continue;
    }
    if (36u <= tile && tile < 52u) {
      tile /= 4u;
      tile += 2u;
      continue;
    }
    if (tile == 52u) {
      tile = 10u;
      continue;
    }
    if (52u < tile && tile < 72u) {
      tile /= 4u;
      tile += 2u;
      continue;
    }
    if (72u <= tile && tile < 88u) {
      tile /= 4u;
      tile += 3u;
      continue;
    }
    if (tile == 88u) {
      tile = 20u;
      continue;
    }
    if (88u < tile) {
      tile /= 4u;
      tile += 3u;
      continue;
    }
    IS_MAJSOUL_FAIR_THROW<std::logic_error>("A logic error.");
  }
  paishan.resize(num_tiles);
  return paishan;
}

} // namespace <unnamed>

int main(int const argc, char const * const * const argv)
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <83|136> <# of paishan>" << std::endl;
    return EXIT_FAILURE;
  }

  std::size_t const num_tiles = boost::lexical_cast<std::size_t>(argv[1u]);
  if (num_tiles != 83u && num_tiles != 136u) {
    std::cerr << "The first argument must be either `83` or `136`, but it is `"
      << num_tiles << "`." << std::endl;
    return EXIT_FAILURE;
  }

  std::size_t const num_paishan = boost::lexical_cast<std::size_t>(argv[2u]);

  std::mt19937 random_number_engine = createRandomNumberEngine();

  for (std::size_t i = 0u; i < num_paishan; ++i) {
    std::vector<std::uint_fast8_t> const paishan = createPaishan(num_tiles, random_number_engine);
    bool is_first = true;
    for (std::uint_fast8_t const tile : paishan) {
      if (!is_first) {
        std::cout << ',';
      }
      std::cout << static_cast<unsigned>(tile);
      is_first = false;
    }
    std::cout << '\n';
  }
}
