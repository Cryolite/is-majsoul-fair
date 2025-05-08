#include "../core/paishan_reader.hpp"
#include "../common/throw.hpp"
#include <boost/math/distributions/chi_squared.hpp>
#include <boost/lexical_cast.hpp>
#include <mutex>
#include <thread>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <cstdint>
#include <cstdlib>


namespace{

using std::placeholders::_1;

std::mutex mtx;

void test(
  unsigned long const num_tiles,
  std::filesystem::path const &path_to_paishans_file,
  std::uint_fast8_t const position,
  unsigned long const num_samples)
{
  std::ifstream ifs(path_to_paishans_file);
  if (!ifs) {
    IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << path_to_paishans_file << ": Failed to open the file.";
  }

  std::array<double, 37u> expected{};
  expected.fill(num_samples / 136.0);
  for (std::uint_fast8_t i = 0u; i < 37u; ++i) {
    switch (i) {
    case 0u:
    case 10u:
    case 20u:
      break;
    case 5u:
    case 15u:
    case 25u:
      expected[i] *= 3.0;
      break;
    default:
      expected[i] *= 4.0;
      break;
    }
  }

  std::vector<unsigned long> counts(37u, 0u);
  for (unsigned long i = 0u; i < num_samples; ++i) {
    std::vector<std::uint_fast8_t> paishan = IsMajsoulFair::readPaishan(num_tiles, ifs);
    std::uint_fast8_t const tile = paishan[position];
    ++counts[tile];
  }

  double chi_square = 0.0;
  for (std::uint_fast8_t i = 0u; i < 37u; ++i) {
    double const diff = static_cast<double>(counts[i]) - expected[i];
    chi_square += (diff * diff) / expected[i];
  }

  double const degrees_of_freedom = static_cast<double>(37u - 1u);
  boost::math::chi_squared_distribution<> chi_square_distribution(degrees_of_freedom);
  double const p_value = 1.0 - boost::math::cdf(chi_square_distribution, chi_square);

  {
    std::lock_guard lock(mtx);
    std::cout << "Position " << static_cast<unsigned>(position) << ": p_value = " << p_value << std::endl;
  }
}

void testThreadMain(
  std::uint_fast8_t const num_tiles,
  std::filesystem::path const &path_to_paishans_file,
  unsigned long const num_samples,
  unsigned const concurrency,
  unsigned const thread_index)
{
  for (std::uint_fast8_t i = thread_index; i < num_tiles; i += concurrency) {
    test(num_tiles, path_to_paishans_file, i, num_samples);
  }
}

} // namespace *unnamed*

int main(int const argc, char const * const * const argv)
{
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " <83 or 136> <path_to_paishans_file> <num_samples>" << std::endl;
    return EXIT_FAILURE;
  }

  unsigned long const num_tiles = boost::lexical_cast<unsigned long>(argv[1]);
  if (num_tiles != 83 && num_tiles != 136) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(_1) << num_tiles << ": The number of tiles must be 83 or 136.";
  }

  std::filesystem::path const path_to_paishans_file(argv[2]);
  if (!std::filesystem::exists(path_to_paishans_file)) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(_1) << path_to_paishans_file << ": The path to test paishans does not exist.";
  }
  if (!std::filesystem::is_regular_file(path_to_paishans_file)) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(_1) << path_to_paishans_file << ": The path to test paishans is not a regular file.";
  }

  unsigned long const num_samples = boost::lexical_cast<unsigned long>(argv[3]);
  if (num_samples == 0u) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(_1) << num_samples << ": The number of samples must be greater than 0.";
  }

  unsigned const concurrency = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  for (unsigned i = 0u; i < concurrency; ++i) {
    threads.emplace_back(
      testThreadMain,
      num_tiles,
      path_to_paishans_file,
      num_samples,
      concurrency,
      i);
  }
  for (auto &thread : threads) {
    thread.join();
  }

  return EXIT_SUCCESS;
}
