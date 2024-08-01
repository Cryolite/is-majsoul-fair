// Copyright (c) 2024 Cryolite
// SPDX-License-Identifier: MIT
// This file is part of https://github.com/Cryolite/tsumonya

#include "type_name.hpp"

#include <string>
#include <memory>
#include <typeinfo>
#include <cstdlib>
#include <cxxabi.h>


namespace IsMajsoulFair{

std::string getTypeName(std::type_info const &ti)
{
  char const * const name = ti.name();
  int status;
  std::unique_ptr<char, void (*)(void *)> p(
    abi::__cxa_demangle(name, NULL, NULL, &status), std::free);
  return { status == 0 ? p.get() : name };
}

} // namespace IsMajsoulFair
