#include "paishan_reader.hpp"

#include "../common/throw.hpp"
#include <istream>
#include <vector>
#include <functional>
#include <stdexcept>
#include <cstdint>


namespace{

using std::placeholders::_1;

} // namespace *unnamed*

namespace IsMajsoulFair{

std::vector<std::uint_fast8_t> readPaishan(std::uint_fast8_t const num_tiles, std::istream &is)
{
  if (is.bad()) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(_1) << "The given input stream is in a bad state.";
  }
  if (is.fail()) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(_1) << "The given input stream is in a fail state.";
  }
  if (is.eof()) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(_1) << "The given input stream is at the end of the file.";
  }

  std::vector<std::uint_fast8_t> paishan;
  paishan.reserve(num_tiles);

  {
    unsigned long tile;
    is >> tile;
    if (is.bad() || is.fail()) {
      IS_MAJSOUL_FAIR_THROW<std::runtime_error>("Failed to read a tile.");
    }
    if (tile > 37u) {
      IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << tile << ": The tile must be in the range [0, 37).";
    }
    if (is.eof()) {
      IS_MAJSOUL_FAIR_THROW<std::runtime_error>("Unexpectedly reached the end of the file.");
    }
    paishan.push_back(tile);
  }

  for (std::uint_fast8_t i = 1; i < num_tiles; ++i) {
    if (is.get() != ',') {
      IS_MAJSOUL_FAIR_THROW<std::runtime_error>("Failed to read a comma.");
    }

    unsigned long tile;
    is >> tile;
    if (is.bad() || is.fail()) {
      IS_MAJSOUL_FAIR_THROW<std::runtime_error>("Failed to read a tile.");
    }
    if (tile > 37u) {
      IS_MAJSOUL_FAIR_THROW<std::runtime_error>(_1) << tile << ": The tile must be in the range [0, 37).";
    }
    paishan.push_back(tile);
  }

  return paishan;
}

} // namespace IsMajsoulFair
