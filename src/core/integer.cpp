// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/is-majsoul-fair.

#include "integer.hpp"

#include "../common/throw.hpp"
#include <stdexcept>
#include <gmp.h>


namespace IsMajsoulFair{

class Integer::Impl_
{
public:
  Impl_()
  {
    mpz_init(value_);
  }

  explicit Impl_(unsigned long const value)
  {
    mpz_init_set_ui(value_, value);
  }

  explicit Impl_(long const value)
  {
    mpz_init_set_si(value_, value);
  }

  Impl_(Impl_ const &other)
  {
    mpz_init_set(value_, other.value_);
  }

  Impl_ &operator+=(Impl_ const &rhs)
  {
    mpz_add(value_, value_, rhs.value_);
    return *this;
  }

  Impl_ &operator+=(unsigned long const rhs)
  {
    mpz_add_ui(value_, value_, rhs);
    return *this;
  }

  Impl_ &operator-=(Impl_ const &rhs)
  {
    mpz_sub(value_, value_, rhs.value_);
    return *this;
  }

  Impl_ &operator-=(unsigned long const rhs)
  {
    mpz_sub_ui(value_, value_, rhs);
    return *this;
  }

  Impl_ &operator*=(Impl_ const &rhs)
  {
    mpz_mul(value_, value_, rhs.value_);
    return *this;
  }

  Impl_ &operator*=(unsigned long const rhs)
  {
    mpz_mul_ui(value_, value_, rhs);
    return *this;
  }

  Impl_ &operator*=(long const rhs)
  {
    mpz_mul_si(value_, value_, rhs);
    return *this;
  }

  Impl_ &operator/=(Impl_ const &rhs)
  {
    mpz_tdiv_q(value_, value_, rhs.value_);
    return *this;
  }

  Impl_ &operator/=(unsigned long const rhs)
  {
    mpz_tdiv_q_ui(value_, value_, rhs);
    return *this;
  }

  Impl_ &operator%=(Impl_ const &rhs)
  {
    mpz_tdiv_r(value_, value_, rhs.value_);
    return *this;
  }

  Impl_ &operator%=(unsigned long const rhs)
  {
    mpz_tdiv_r_ui(value_, value_, rhs);
    return *this;
  }

  void inplacePow(unsigned long const exponent)
  {
    mpz_pow_ui(value_, value_, exponent);
  }

  void setToRandom(gmp_randstate_t state, Impl_ const &upper)
  {
    mpz_urandomm(value_, state, upper.value_);
  }

  explicit operator unsigned long() const
  {
    return mpz_get_ui(value_);
  }

  bool operator==(Impl_ const &rhs) const
  {
    return mpz_cmp(value_, rhs.value_) == 0;
  }

  bool operator==(unsigned long const rhs) const
  {
    return mpz_cmp_ui(value_, rhs) == 0;
  }

  bool operator==(long const rhs) const
  {
    return mpz_cmp_si(value_, rhs) == 0;
  }

  bool operator<(Impl_ const &rhs) const
  {
    return mpz_cmp(value_, rhs.value_) < 0;
  }

  bool operator<(unsigned long const rhs) const
  {
    return mpz_cmp_ui(value_, rhs) < 0;
  }

  bool operator<(long const rhs) const
  {
    return mpz_cmp_si(value_, rhs) < 0;
  }

  bool operator<=(unsigned long const rhs) const
  {
    return mpz_cmp_ui(value_, rhs) <= 0;
  }

  bool operator<=(long const rhs) const
  {
    return mpz_cmp_si(value_, rhs) <= 0;
  }

  bool operator>(unsigned long const rhs) const
  {
    return mpz_cmp_ui(value_, rhs) > 0;
  }

  bool operator>(long const rhs) const
  {
    return mpz_cmp_si(value_, rhs) > 0;
  }

  bool operator>=(unsigned long const rhs) const
  {
    return mpz_cmp_ui(value_, rhs) >= 0;
  }

  bool operator>=(long const rhs) const
  {
    return mpz_cmp_si(value_, rhs) >= 0;
  }

  ~Impl_()
  {
    mpz_clear(value_);
  }

private:
  mpz_t value_;
}; // class Integer::Impl_

class IntegerRandomState::Impl_
{
public:
  Impl_()
  {
    gmp_randinit_mt(state_);
  }

  Impl_(Impl_ const &other) = delete;

  Impl_(Impl_ &&other) = delete;

  Impl_ &operator=(Impl_ const &other) = delete;

  Impl_ &operator=(Impl_ &&other) = delete;

  ~Impl_()
  {
    gmp_randclear(state_);
  }

  gmp_randstate_t &get() noexcept
  {
    return state_;
  }

private:
  gmp_randstate_t state_;
}; // class IntegerRandomState::Impl_

Integer::Integer()
  : p_impl_(std::make_shared<Impl_>())
{}

Integer::Integer(unsigned long const value)
  : p_impl_(std::make_shared<Impl_>(value))
{}

Integer::Integer(long const value)
  : p_impl_(std::make_shared<Impl_>(value))
{}

Integer::Integer(Integer const &other)
  : p_impl_(std::make_shared<Impl_>(*other.p_impl_))
{}

Integer::Integer(Integer &&other) noexcept
  : p_impl_(std::move(other.p_impl_))
{
  other.p_impl_ = nullptr;
}

void Integer::swap(Integer &other) noexcept
{
  p_impl_.swap(other.p_impl_);
}

void Integer::swap(Integer &&other) noexcept
{
  p_impl_.swap(other.p_impl_);
}

Integer &Integer::operator=(Integer const &other)
{
  Integer(other).swap(*this);
  return *this;
}

Integer &Integer::operator=(Integer &&other) noexcept
{
  Integer(std::move(other)).swap(*this);
  return *this;
}

Integer &Integer::operator=(unsigned long const value)
{
  Integer(value).swap(*this);
  return *this;
}

Integer &Integer::operator=(long const value)
{
  Integer(value).swap(*this);
  return *this;
}

Integer &Integer::operator++()
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  return *this += 1ul;
}

Integer Integer::operator++(int)
{
  Integer result(*this);
  ++*this;
  return result;
}

Integer &Integer::operator--()
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  return *this -= 1ul;
}

Integer Integer::operator--(int)
{
  Integer result(*this);
  --*this;
  return result;
}

Integer &Integer::operator+=(Integer const &rhs)
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  if (!rhs.p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`rhs.p_impl_` is `nullptr`.");
  }
  *p_impl_ += *rhs.p_impl_;
  return *this;
}

Integer &Integer::operator+=(unsigned long const rhs)
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  *p_impl_ += rhs;
  return *this;
}

Integer Integer::operator+(Integer const &rhs) const
{
  return Integer(*this) += rhs;
}

Integer Integer::operator+(unsigned long const rhs) const
{
  return Integer(*this) += rhs;
}

Integer &Integer::operator-=(Integer const &rhs)
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  if (!rhs.p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`rhs.p_impl_` is `nullptr`.");
  }
  *p_impl_ -= *rhs.p_impl_;
  return *this;
}

Integer &Integer::operator-=(unsigned long const rhs)
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  *p_impl_ -= rhs;
  return *this;
}

Integer Integer::operator-(Integer const &rhs) const
{
  return Integer(*this) -= rhs;
}

Integer Integer::operator-(unsigned long const rhs) const
{
  return Integer(*this) -= rhs;
}

Integer &Integer::operator*=(Integer const &rhs)
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  if (!rhs.p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`rhs.p_impl_` is `nullptr`.");
  }
  *p_impl_ *= *rhs.p_impl_;
  return *this;
}

Integer &Integer::operator*=(unsigned long const rhs)
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  *p_impl_ *= rhs;
  return *this;
}

Integer &Integer::operator*=(long const rhs)
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  *p_impl_ *= rhs;
  return *this;
}

Integer Integer::operator*(Integer const &rhs) const
{
  return Integer(*this) *= rhs;
}

Integer Integer::operator*(unsigned long const rhs) const
{
  return Integer(*this) *= rhs;
}

Integer Integer::operator*(long const rhs) const
{
  return Integer(*this) *= rhs;
}

Integer &Integer::operator/=(Integer const &rhs)
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  if (!rhs.p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`rhs.p_impl_` is `nullptr`.");
  }
  *p_impl_ /= *rhs.p_impl_;
  return *this;
}

Integer &Integer::operator/=(unsigned long const rhs)
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  *p_impl_ /= rhs;
  return *this;
}

Integer Integer::operator/(Integer const &rhs) const
{
  return Integer(*this) /= rhs;
}

Integer Integer::operator/(unsigned long const rhs) const
{
  return Integer(*this) /= rhs;
}

Integer &Integer::operator%=(Integer const &rhs)
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  if (!rhs.p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`rhs.p_impl_` is `nullptr`.");
  }
  *p_impl_ %= *rhs.p_impl_;
  return *this;
}

Integer &Integer::operator%=(unsigned long const rhs)
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  *p_impl_ %= rhs;
  return *this;
}

Integer Integer::operator%(Integer const &rhs) const
{
  return Integer(*this) %= rhs;
}

Integer Integer::operator%(unsigned long const rhs) const
{
  return Integer(*this) %= rhs;
}

Integer &Integer::inplacePow(unsigned long const exponent)
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  p_impl_->inplacePow(exponent);
  return *this;
}

Integer Integer::pow(unsigned long const exponent) const
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  return Integer(*this).inplacePow(exponent);
}

Integer &Integer::setToRandom(
  IntegerRandomState &state, Integer const &lower, Integer const &upper)
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  if (!state.p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`state.p_impl_` is `nullptr`.");
  }
  Integer const range = upper - lower;
  p_impl_->setToRandom(state.p_impl_->get(), *range.p_impl_);

  return *this += lower;
}

Integer::operator unsigned long() const
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  if (*this < 0ul) {
    IS_MAJSOUL_FAIR_THROW<std::underflow_error>("Underflow occurred.");
  }
  if (*this > Integer(ULONG_MAX)) {
    IS_MAJSOUL_FAIR_THROW<std::overflow_error>("Overflow occurred.");
  }
  return static_cast<unsigned long>(*p_impl_);
}

bool Integer::operator==(Integer const &rhs) const
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  if (!rhs.p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`rhs.p_impl_` is `nullptr`.");
  }
  return *p_impl_ == *rhs.p_impl_;
}

bool Integer::operator==(unsigned long const rhs) const
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  return *p_impl_ == rhs;
}

bool Integer::operator==(long const rhs) const
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  return *p_impl_ == rhs;
}

bool Integer::operator!=(Integer const &rhs) const
{
  return !(*this == rhs);
}

bool Integer::operator!=(unsigned long const rhs) const
{
  return !(*this == rhs);
}

bool Integer::operator!=(long const rhs) const
{
  return !(*this == rhs);
}

bool Integer::operator<(Integer const &rhs) const
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  if (!rhs.p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`rhs.p_impl_` is `nullptr`.");
  }
  return *p_impl_ < *rhs.p_impl_;
}

bool Integer::operator<(unsigned long const rhs) const
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  return *p_impl_ < rhs;
}

bool Integer::operator<(long const rhs) const
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  return *p_impl_ < rhs;
}

bool Integer::operator<=(Integer const &rhs) const
{
  return !(rhs < *this);
}

bool Integer::operator<=(unsigned long const rhs) const
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  return *p_impl_ <= rhs;
}

bool Integer::operator<=(long const rhs) const
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }  
  return *p_impl_ <= rhs;
}

bool Integer::operator>(Integer const &rhs) const
{
  return rhs < *this;
}

bool Integer::operator>(unsigned long const rhs) const
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  return *p_impl_ > rhs;
}

bool Integer::operator>(long const rhs) const
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }  
  return *p_impl_ > rhs;
}

bool Integer::operator>=(Integer const &rhs) const
{
  return !(*this < rhs);
}

bool Integer::operator>=(unsigned long const rhs) const
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  return *p_impl_ >= rhs;
}

bool Integer::operator>=(long const rhs) const
{
  if (!p_impl_) {
    IS_MAJSOUL_FAIR_THROW<std::invalid_argument>("`p_impl_` is `nullptr`.");
  }
  return *p_impl_ >= rhs;
}

void swap(Integer &lhs, Integer &rhs) noexcept
{
  lhs.swap(rhs);
}

void swap(Integer &lhs, Integer &&rhs) noexcept
{
  lhs.swap(rhs);
}

void swap(Integer &&lhs, Integer &rhs) noexcept
{
  lhs.swap(rhs);
}

Integer operator+(unsigned long const lhs, Integer const &rhs)
{
  return rhs + lhs;
}

Integer operator-(unsigned long const lhs, Integer const &rhs)
{
  return Integer(lhs) - rhs;
}

Integer operator*(unsigned long const lhs, Integer const &rhs)
{
  return rhs * lhs;
}

Integer operator*(long const lhs, Integer const &rhs)
{
  return rhs * lhs;
}

Integer operator/(unsigned long const lhs, Integer const &rhs)
{
  return Integer(lhs) / rhs;
}

Integer operator%(unsigned long const lhs, Integer const &rhs)
{
  return Integer(lhs) % rhs;
}

bool operator==(unsigned long const lhs, Integer const &rhs)
{
  return rhs == lhs;
}

bool operator==(long const lhs, Integer const &rhs)
{
  return rhs == lhs;
}

bool operator!=(unsigned long const lhs, Integer const &rhs)
{
  return rhs != lhs;
}

bool operator!=(long const lhs, Integer const &rhs)
{
  return rhs != lhs;
}

bool operator<(unsigned long const lhs, Integer const &rhs)
{
  return rhs > lhs;
}

bool operator<(long const lhs, Integer const &rhs)
{
  return rhs > lhs;
}

bool operator<=(unsigned long const lhs, Integer const &rhs)
{
  return rhs >= lhs;
}

bool operator<=(long const lhs, Integer const &rhs)
{
  return rhs >= lhs;
}

bool operator>(unsigned long const lhs, Integer const &rhs)
{
  return rhs < lhs;
}

bool operator>(long const lhs, Integer const &rhs)
{
  return rhs < lhs;
}

bool operator>=(unsigned long const lhs, Integer const &rhs)
{
  return rhs <= lhs;
}

bool operator>=(long const lhs, Integer const &rhs)
{
  return rhs <= lhs;
}

IntegerRandomState::IntegerRandomState()
  : p_impl_(std::make_shared<Impl_>())
{}

IntegerRandomState::IntegerRandomState(IntegerRandomState &&other) noexcept
  : p_impl_(std::move(other.p_impl_))
{
  other.p_impl_ = nullptr;
}

void IntegerRandomState::swap(IntegerRandomState &other) noexcept
{
  p_impl_.swap(other.p_impl_);
}

void IntegerRandomState::swap(IntegerRandomState &&other) noexcept
{
  p_impl_.swap(other.p_impl_);
}

IntegerRandomState &IntegerRandomState::operator=(IntegerRandomState &&other) noexcept
{
  IntegerRandomState(std::move(other)).swap(*this);
  return *this;
}

void swap(IntegerRandomState &lhs, IntegerRandomState &rhs) noexcept
{
  lhs.swap(rhs);
}

void swap(IntegerRandomState &lhs, IntegerRandomState &&rhs) noexcept
{
  lhs.swap(rhs);
}

void swap(IntegerRandomState &&lhs, IntegerRandomState &rhs) noexcept
{
  lhs.swap(rhs);
}

} // namespace IsMajsoulFair
