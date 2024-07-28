#if !defined(CORE_INTERVAL_TO_BINARY_HPP)
#define CORE_INTERVAL_TO_BINARY_HPP

#include "interval.hpp"
#include <vector>
#include <cstddef>


namespace IsMajsoulFair{

std::vector<unsigned char> intervalToBinary(Interval const &interval, std::size_t num_bits);

} // namespace IsMajsoulFair

#endif // !defined(CORE_INTERVAL_TO_BINARY_HPP)
