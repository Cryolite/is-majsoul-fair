#if !defined(CORE_INTEGER_HPP)
#define CORE_INTEGER_HPP

#include <memory>


namespace IsMajsoulFair{

class Integer
{
public:
  Integer();

  explicit Integer(unsigned long value);

  explicit Integer(long value);

  Integer(Integer const &other);

  Integer(Integer &&other) noexcept;

  void swap(Integer &other) noexcept;

  void swap(Integer &&other) noexcept;

  Integer &operator=(Integer const &other);

  Integer &operator=(Integer &&other) noexcept;

  Integer &operator=(unsigned long value);

  Integer &operator=(long value);

  Integer &operator+=(Integer const &rhs);

  Integer &operator+=(unsigned long rhs);

  Integer operator+(Integer const &rhs) const;

  Integer operator+(unsigned long rhs) const;

  Integer &operator-=(Integer const &rhs);

  Integer &operator-=(unsigned long rhs);

  Integer operator-(Integer const &rhs) const;

  Integer operator-(unsigned long rhs) const;

  Integer &operator*=(Integer const &rhs);

  Integer &operator*=(unsigned long rhs);

  Integer &operator*=(long rhs);

  Integer operator*(Integer const &rhs) const;

  Integer operator*(unsigned long rhs) const;

  Integer operator*(long rhs) const;

  Integer &operator/=(Integer const &rhs);

  Integer &operator/=(unsigned long rhs);

  Integer operator/(Integer const &rhs) const;

  Integer operator/(unsigned long rhs) const;

  Integer &inplacePow(unsigned long exponent);

  Integer pow(unsigned long exponent) const;

  bool operator==(Integer const &rhs) const;

  bool operator==(unsigned long rhs) const;

  bool operator==(long rhs) const;

  bool operator!=(Integer const &rhs) const;

  bool operator!=(unsigned long rhs) const;

  bool operator!=(long rhs) const;

  bool operator<(Integer const &rhs) const;

  bool operator<(unsigned long rhs) const;

  bool operator<(long rhs) const;

  bool operator<=(Integer const &rhs) const;

  bool operator<=(unsigned long rhs) const;

  bool operator<=(long rhs) const;

  bool operator>(Integer const &rhs) const;

  bool operator>(unsigned long rhs) const;

  bool operator>(long rhs) const;

  bool operator>=(Integer const &rhs) const;

  bool operator>=(unsigned long rhs) const;

  bool operator>=(long rhs) const;

private:
  class Impl_;
  std::shared_ptr<Impl_> p_impl_;
}; // class Integer

void swap(Integer &lhs, Integer &rhs) noexcept;

void swap(Integer &lhs, Integer &&rhs) noexcept;

void swap(Integer &&lhs, Integer &rhs) noexcept;

bool operator==(unsigned long lhs, Integer const &rhs);

bool operator==(long lhs, Integer const &rhs);

bool operator!=(unsigned long lhs, Integer const &rhs);

bool operator!=(long lhs, Integer const &rhs);

bool operator<(unsigned long lhs, Integer const &rhs);

bool operator<(long lhs, Integer const &rhs);

bool operator<=(unsigned long lhs, Integer const &rhs);

bool operator<=(long lhs, Integer const &rhs);

bool operator>(unsigned long lhs, Integer const &rhs);

bool operator>(long lhs, Integer const &rhs);

bool operator>=(unsigned long lhs, Integer const &rhs);

bool operator>=(long lhs, Integer const &rhs);

} // namespace IsMajsoulFair

#endif // !defined(CORE_INTEGER_HPP)
