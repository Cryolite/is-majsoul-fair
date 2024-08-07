// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/is-majsoul-fair.

#include "common/throw.hpp"
#include <nyanten/calculator.hpp>
#include <boost/lexical_cast.hpp>
#include <atomic>
#include <thread>
#include <random>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <functional>
#include <stdexcept>
#include <cstdint>
#include <cstdlib>
#include <cstddef>


namespace{

std::mt19937 createRandomNumberEngine() {
  std::random_device random_device;
  std::array<std::mt19937::result_type, std::mt19937::state_size> seed;
  std::generate(seed.begin(), seed.end(), std::ref(random_device));
  std::seed_seq seed_sequence(seed.cbegin(), seed.cend());
  return std::mt19937(seed_sequence);
}

std::array<std::uint_fast8_t, 34u> createQipai(
  std::uint_fast8_t const num_tiles, std::mt19937 &random_number_engine)
{
  std::vector<std::uint_fast8_t> paishan(136u, 0u);
  std::iota(paishan.begin(), paishan.end(), 0u);
  std::shuffle(paishan.begin(), paishan.end(), random_number_engine);
  paishan.resize(num_tiles);

  std::array<std::uint_fast8_t, 34u> qipai{};
  for (std::uint_fast8_t const tile : paishan) {
    ++qipai[tile / 4u];
  }
  return qipai;
}

void threadMain(
  std::uint_fast8_t const num_tiles,
  std::size_t const num_simulations,
  Nyanten::Calculator const &calculator,
  std::array<std::atomic_size_t, 8u> &table)
{
  std::mt19937 random_number_engine = createRandomNumberEngine();

  for (std::size_t i = 0; i < num_simulations; ++i) {
    std::array<std::uint_fast8_t, 34u> const qipai = createQipai(num_tiles, random_number_engine);
    std::uint_fast8_t const replacement_number = calculator(qipai);
    ++table[replacement_number];
  }
}

} // namespace <anonymous>

int main(int const argc, char const * const * const argv) {
  if (argc != 4 && argc != 5) {
    std::cerr << "Usage: " << argv[0]
      << " <nyanten-map-file> <13|14> <# of simulations> [# of threads]" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::filesystem::path const nyanten_map_path(argv[1]);
  if (!std::filesystem::exists(nyanten_map_path)) {
    std::cerr << "Error: `" << nyanten_map_path << "` does not exist." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::uint_fast8_t const num_tiles = boost::lexical_cast<unsigned>(argv[2]);
  if (num_tiles != 13 && num_tiles != 14) {
    std::cerr << "Error: The number of tiles must be 13 or 14." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::size_t const num_simulations = boost::lexical_cast<std::size_t>(argv[3]);

  std::size_t const num_threads = [&]() -> std::size_t {
    if (argc == 5) {
      std::size_t const num_threads = boost::lexical_cast<std::size_t>(argv[4]);
      if (num_threads == 0) {
        return std::thread::hardware_concurrency();
      }
      return num_threads;
    }
    return 1u;
  }();

  Nyanten::Calculator calculator(nyanten_map_path);

  std::array<std::atomic_size_t, 8u> table{};
  if (num_threads == 1u) {
    threadMain(num_tiles, num_simulations, calculator, table);
  }
  else {
    std::vector<std::jthread> threads;
    for (std::size_t i = 0u; i < 10u; ++i) {
      threads.emplace_back(
        &threadMain, num_tiles, num_simulations, std::ref(calculator), std::ref(table));
    }
    for (std::jthread &thread : threads) {
      thread.join();
    }
  }
  for (std::uint_fast8_t i = 0u; i < table.size(); ++i) {
    std::cout << static_cast<unsigned>(i) << ": " << table[i].load() << std::endl;
  }

  return EXIT_SUCCESS;
}
