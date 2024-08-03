# Copyright (c) 2024 Cryolite
# SPDX-License-Identifier: MIT
# This file is part of https://github.com/Cryolite/tsumonya

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
  core/permutation_to_interval.cpp
  core/interval.cpp
  core/integer.cpp)
target_link_libraries(core
  PRIVATE common
  PRIVATE gmp)

add_executable(paishan
  paishan.cpp)
target_link_libraries(paishan
  PRIVATE common
  PRIVATE Boost::headers)

add_executable(paishan_to_binary
  paishan_to_binary.cpp)
target_link_libraries(paishan_to_binary
  PRIVATE core
  PRIVATE common
  PRIVATE Boost::headers)
