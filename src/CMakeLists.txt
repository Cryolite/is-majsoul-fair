# Copyright (c) 2024 Cryolite
# SPDX-License-Identifier: MIT
# This file is part of https://github.com/Cryolite/is-majsoul-fair.

add_library(common
  common/throw.cpp
  common/type_name.cpp
  common/mahjongsoul.pb.cc)
target_link_libraries(common
  PRIVATE Boost::stacktrace_backtrace
  PRIVATE Boost::headers
  PRIVATE ${Protobuf_LIBRARIES}
  PRIVATE absl::log_internal_check_op
  PRIVATE absl::log_internal_conditions
  PRIVATE absl::raw_hash_set
  PRIVATE absl::statusor
  PRIVATE utf8_validity)

add_library(core
  core/interval_to_binary.cpp
  core/interval_to_entropy.cpp
  core/covering_binary_interval.cpp
  core/permutation_to_interval.cpp
  core/interval.cpp
  core/integer.cpp)
target_link_libraries(core
  PRIVATE common
  PRIVATE gmp)

add_executable(shanten
  shanten.cpp)
target_link_libraries(shanten
  PRIVATE common
  PRIVATE Boost::headers)

add_executable(unbiased_paishan
  unbiased_paishan.cpp)
target_link_libraries(unbiased_paishan
  PRIVATE common
  PRIVATE Boost::headers)

add_executable(paishan
  paishan.cpp)
target_link_libraries(paishan
  PRIVATE common
  PRIVATE Boost::headers)

add_executable(paishan_to_entropy
  paishan_to_entropy.cpp)
target_link_libraries(paishan_to_entropy
  PRIVATE core
  PRIVATE common
  PRIVATE Boost::headers)

add_executable(paishan_to_binary
  paishan_to_binary.cpp)
target_link_libraries(paishan_to_binary
  PRIVATE core
  PRIVATE common
  PRIVATE Boost::headers)
