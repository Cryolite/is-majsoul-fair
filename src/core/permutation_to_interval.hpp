#if !defined(CORE_PERMUTATION_TO_INTERVAL_HPP)
#define CORE_PERMUTATION_TO_INTERVAL_HPP

#include "interval.hpp"
#include <vector>
#include <cstdint>


namespace IsMajsoulFair{

IsMajsoulFair::Interval permutationToInterval(std::vector<std::uint_fast8_t> const &permutation);

} // namespace IsMajsoulFair

#endif // !defined(CORE_PERMUTATION_TO_INTERVAL_HPP)
