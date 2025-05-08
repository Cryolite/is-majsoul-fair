#if !defined(CORE_FAIR_PAISHAN_HPP_INCLUDE_GUARD)
#define CORE_FAIR_PAISHAN_HPP_INCLUDE_GUARD

#include <random>
#include <vector>
#include <cstdint>


namespace IsMajsoulFair{

std::vector<std::uint_fast8_t> generateFairPaishan(
  std::mt19937 &random_number_engine,
  std::uint_fast8_t const num_tiles);

} // namespace IsMajsoulFair

#endif // !defined(CORE_FAIR_PAISHAN_HPP_INCLUDE_GUARD)
