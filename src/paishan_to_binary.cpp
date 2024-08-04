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
  for (unsigned char bit : binary) {
    std::cout << static_cast<unsigned>(bit);
  }
}

} // namespace <unnamed>

int main(int const argc, char const * const * const argv)
{
  if (argc != 3) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("The number of arguments is invalid.");
  }

  IsMajsoulFair::IntegerRandomState state;

  std::filesystem::path const path(argv[1]);
  std::ifstream ifs(path);
  if (!ifs) {
    IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << path.string() << ": Failed to open.";
  }

  std::size_t const num_bits = boost::lexical_cast<std::size_t>(argv[2]);

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
