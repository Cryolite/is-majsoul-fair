// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/is-majsoul-fair.

#include "core/interval_to_binary.hpp"
#include "core/permutation_to_interval.hpp"
#include "core/interval.hpp"
#include "core/integer.hpp"
#include "common/throw.hpp"
#include <boost/lexical_cast.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string_view>
#include <string>
#include <functional>
#include <cstdint>
#include <cstdlib>


namespace{

using std::placeholders::_1;

void paishanToBinary(
  std::vector<unsigned char> const &paishan,
  std::size_t const num_bits,
  IsMajsoulFair::IntegerRandomState &state)
{
  IsMajsoulFair::Interval const interval = IsMajsoulFair::permutationToInterval(paishan);
  std::vector<unsigned char> const binary = IsMajsoulFair::intervalToBinary(interval, num_bits, state);
  if (binary.size() % 8u != 0u) {
    IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << binary.size();
  }
  for (std::size_t i = 0u; i < binary.size(); i += 8u) {
    std::uint8_t const byte = binary[i + 0u] << 7u
                            | binary[i + 1u] << 6u
                            | binary[i + 2u] << 5u
                            | binary[i + 3u] << 4u
                            | binary[i + 4u] << 3u
                            | binary[i + 5u] << 2u
                            | binary[i + 6u] << 1u
                            | binary[i + 7u] << 0u;
    std::cout << byte;
  }
  std::cout << std::flush;
}

} // namespace <unnamed>

int main(int const argc, char const * const * const argv)
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <PATH TO PAISHAN FILE> <# OF BITS PER PAISHAN>" << std::endl;
    return EXIT_FAILURE;
  }

  IsMajsoulFair::IntegerRandomState state;

  std::filesystem::path const path(argv[1]);
  std::ifstream ifs(path);
  if (!ifs) {
    IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << path.string() << ": Failed to open.";
  }

  long long const num_bits = boost::lexical_cast<long long>(argv[2]);
  if (num_bits <= 0) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(_1) << num_bits << ": An invalid number of bits.";
  }
  if (num_bits % 8 != 0) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(_1)
      << num_bits << ": The number of bits must be a multiple of 8.";
  }

  while (true) {
    std::string line;
    std::getline(ifs, line);
    if (ifs.bad()) {
      IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << path.string() << ": Failed to read.";
    }

    if (!line.empty()) {
      std::vector<unsigned char> paishan;
      for (auto e : line | std::ranges::views::split(',')) {
        std::string_view sv{e.cbegin(), e.cend()};
        unsigned long const tile = boost::lexical_cast<unsigned long>(sv);
        if (tile >= 37u) {
          IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(_1) << static_cast<unsigned>(tile);
        }
        paishan.push_back(tile);
      }
      if (paishan.size() != 83u && paishan.size() != 136u) {
        IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(_1) << paishan.size();
      }

      paishanToBinary(paishan, num_bits, state);
      continue;
    }

    if (ifs.eof()) {
      break;
    }

    IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << path.string() << ": Failed to read.";
  }
}
