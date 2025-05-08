# Copyright (c) 2025 Cryolite
# SPDX-License-Identifier: MIT
# This file is part of https://github.com/Cryolite/is-majsoul-fair.

add_executable(chi_square_1
  chi_square_1.cpp)
target_link_libraries(chi_square_1
  PRIVATE core
  PRIVATE common
  PRIVATE Boost::headers)

add_executable(chi_square_2
  chi_square_2.cpp)
target_link_libraries(chi_square_2
  PRIVATE core
  PRIVATE common
  PRIVATE Boost::headers)
