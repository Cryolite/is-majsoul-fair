#include "interval.hpp"

#include "../common/throw.hpp"
#include <stdexcept>


namespace IsMajsoulFair{

Interval::Interval(
  IsMajsoulFair::Integer const &denominator,
  IsMajsoulFair::Integer const &lower,
  IsMajsoulFair::Integer const &upper)
  : denominator_(denominator), lower_(lower), upper_(upper)
{
  if (lower_ > denominator_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`lower` must be less than or equal to `denominator`.");
  }
  if (upper_ > denominator_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`upper` must be less than or equal to `denominator`.");
  }
  if (upper_ < lower_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`upper` must be greater than or equal to `lower`.");
  }
}

} // namespace IsMajsoulFair
