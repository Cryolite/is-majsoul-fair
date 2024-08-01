// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/tsumonya

#include "interval.hpp"

#include "../common/throw.hpp"
#include <stdexcept>


namespace IsMajsoulFair{

Interval::Interval(
  IsMajsoulFair::Integer const &denominator,
  IsMajsoulFair::Integer const &lower_numerator,
  IsMajsoulFair::Integer const &upper_numerator)
  : denominator_(denominator), lower_numerator_(lower_numerator), upper_numerator_(upper_numerator)
{
  if (denominator_ <= 0ul) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`denominator` must be positive.");
  }
  if (lower_numerator_ < 0ul) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`lower_numerator` must be non-negative.");
  }
  if (upper_numerator_ < 0ul) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`upper_numerator` must be non-negative.");
  }
  if (lower_numerator_ > denominator_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(
      "`lower_numerator` must be less than or equal to `denominator`.");
  }
  if (upper_numerator_ > denominator_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(
      "`upper_numerator` must be less than or equal to `denominator`.");
  }
  if (upper_numerator_ < lower_numerator_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>(
      "`upper_numerator` must be greater than or equal to `lower_numerator`.");
  }
}

Interval::Interval(Interval const &other) = default;

Interval::Interval(Interval &&other) noexcept = default;

void Interval::swap(Interval &other) noexcept
{
  using std::swap;
  swap(denominator_, other.denominator_);
  swap(lower_numerator_, other.lower_numerator_);
  swap(upper_numerator_, other.upper_numerator_);
}

void Interval::swap(Interval &&other) noexcept
{
  swap(other);
}

Interval &Interval::operator=(Interval const &other)
{
  Interval(other).swap(*this);
  return *this;
}

Interval &Interval::operator=(Interval &&other) noexcept
{
  Interval(std::move(other)).swap(*this);
  return *this;
}

IsMajsoulFair::Integer const &Interval::getDenominator() const noexcept
{
  return denominator_;
}

IsMajsoulFair::Integer const &Interval::getLowerNumerator() const noexcept
{
  return lower_numerator_;
}

IsMajsoulFair::Integer const &Interval::getUpperNumerator() const noexcept
{
  return upper_numerator_;
}

void swap(Interval &lhs, Interval &rhs) noexcept
{
  lhs.swap(rhs);
}

void swap(Interval &lhs, Interval &&rhs) noexcept
{
  lhs.swap(rhs);
}

void swap(Interval &&lhs, Interval &rhs) noexcept
{
  lhs.swap(rhs);
}

} // namespace IsMajsoulFair
