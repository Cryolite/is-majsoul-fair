#if !defined(CORE_PAISHAN_READER_HPP_INCLUDE_GUARD)
#define CORE_PAISHAN_READER_HPP_INCLUDE_GUARD

#include <iosfwd>
#include <vector>
#include <cstdint>


namespace IsMajsoulFair{

std::vector<std::uint_fast8_t> readPaishan(std::uint_fast8_t const num_tiles, std::istream &is);

} // namespace IsMajsoulFair

#endif // !defined(CORE_PAISHAN_READER_HPP_INCLUDE_GUARD)
