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

add_executable(paishan
  paishan.cpp)
target_link_libraries(paishan
  PRIVATE common
  PRIVATE Boost::headers)
