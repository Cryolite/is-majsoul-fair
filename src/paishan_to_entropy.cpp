// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/is-majsoul-fair.

#include "core/interval_to_entropy.hpp"
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
#include <stdexcept>
#include <cstdint>
#include <cstdlib>


namespace{

using std::placeholders::_1;

double paishanToEntropy(std::vector<std::uint_fast8_t> const &paishan, std::size_t const num_bits)
{
  IsMajsoulFair::Interval const interval = IsMajsoulFair::permutationToInterval(paishan);
  return IsMajsoulFair::intervalToEntropy(interval, num_bits);
}

} // namespace <unnamed>

int main(int const argc, char const * const * const argv)
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <path to paishan file> <# of bits per paishan>" << std::endl;
    return EXIT_FAILURE;
  }

  std::filesystem::path const path(argv[1]);
  std::ifstream ifs(path);
  if (!ifs) {
    IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << path.string() << ": Failed to open.";
  }

  std::size_t const num_bits = boost::lexical_cast<std::size_t>(argv[2]);

  std::size_t num_paishan = 0u;
  double entropy = 0.0;
  while (true) {
    std::string line;
    std::getline(ifs, line);
    if (ifs.bad()) {
      IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << path.string() << ": Failed to read.";
    }

    if (!line.empty()) {
      std::vector<std::uint_fast8_t> paishan;
      for (auto e : line | std::ranges::views::split(',')) {
        std::string_view sv{e.cbegin(), e.cend()};
        unsigned const tile = boost::lexical_cast<unsigned>(sv);
        if (tile >= 37u) {
          IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(_1) << tile;
        }
        paishan.push_back(tile);
      }
      if (paishan.size() != 83u && paishan.size() != 136u) {
        IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(_1) << paishan.size();
      }

      entropy += paishanToEntropy(paishan, num_bits);
      ++num_paishan;
      continue;
    }

    if (ifs.eof()) {
      break;
    }

    IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << path.string() << ": Failed to read.";
  }

  std::cout << entropy / num_paishan << std::endl;
}
