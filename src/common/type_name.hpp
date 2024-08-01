// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/tsumonya

#if !defined(COMMON_TYPE_NAME_HPP_INCLUDE_GUARD)
#define COMMON_TYPE_NAME_HPP_INCLUDE_GUARD

#include <string>
#include <typeinfo>


namespace IsMajsoulFair{

std::string getTypeName(std::type_info const &ti);

template<typename T>
std::string getTypeName(T const &x)
{
  return IsMajsoulFair::getTypeName(typeid(x));
}

} // namespace IsMajsoulFair

#endif // !defined(COMMON_TYPE_NAME_HPP_INCLUDE_GUARD)
