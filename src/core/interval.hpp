// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/tsumonya

#if !defined(CORE_INTERVAL_HPP)
#define CORE_INTERVAL_HPP

#include "integer.hpp"


namespace IsMajsoulFair{

class Interval
{
public:
  Interval(
    IsMajsoulFair::Integer const &denominator,
    IsMajsoulFair::Integer const &lower_numerator,
    IsMajsoulFair::Integer const &upper_numerator);

  Interval(Interval const &other);

  Interval(Interval &&other) noexcept;

  void swap(Interval &other) noexcept;

  void swap(Interval &&other) noexcept;

  Interval &operator=(Interval const &other);

  Interval &operator=(Interval &&other) noexcept;

  IsMajsoulFair::Integer const &getDenominator() const noexcept;

  IsMajsoulFair::Integer const &getLowerNumerator() const noexcept;

  IsMajsoulFair::Integer const &getUpperNumerator() const noexcept;

private:
  IsMajsoulFair::Integer denominator_;
  IsMajsoulFair::Integer lower_numerator_;
  IsMajsoulFair::Integer upper_numerator_;
}; // class Interval

void swap(Interval &lhs, Interval &rhs) noexcept;

void swap(Interval &lhs, Interval &&rhs) noexcept;

void swap(Interval &&lhs, Interval &rhs) noexcept;

} // namespace IsMajsoulFair

#endif // !defined(CORE_INTERVAL_HPP)
