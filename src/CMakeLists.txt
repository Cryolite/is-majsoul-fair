# Copyright (c) 2024 Cryolite
# SPDX-License-Identifier: MIT
# This file is part of https://github.com/Cryolite/is-majsoul-fair.

add_library(common
  common/throw.cpp
  common/type_name.cpp
  common/mahjongsoul.pb.cc)
target_link_libraries(common
  PRIVATE Boost::stacktrace_backtrace
  INTERFACE Boost::headers
  PRIVATE protobuf::libprotobuf
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
  core/integer.cpp
  core/paishan_reader.cpp
  core/fair_paishan.cpp)
target_link_libraries(core
  PRIVATE common
  PRIVATE Boost::headers
  PRIVATE gmp)

add_executable(fair_shanten_distribution
  fair_shanten_distribution.cpp)
target_link_libraries(fair_shanten_distribution
  PRIVATE common
  PRIVATE Boost::headers)

add_executable(fair_paishan
  fair_paishan.cpp)
target_link_libraries(fair_paishan
  PRIVATE common
  PRIVATE Boost::headers)

add_executable(parse_game_records
  parse_game_records.cpp)
target_link_libraries(parse_game_records
  PRIVATE common)

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

add_subdirectory(original)
