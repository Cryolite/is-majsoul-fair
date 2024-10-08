# Copyright (c) 2024 Cryolite
# SPDX-License-Identifier: MIT
# This file is part of https://github.com/Cryolite/is-majsoul-fair.

cmake_minimum_required(VERSION 3.25.0)
project(is-majsoul-fair)

enable_testing()

message("CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")

set(CMAKE_CXX_STANDARD 23)

if ((NOT DEFINED NYANTEN_ROOT) OR (NYANTEN_ROOT STREQUAL ""))
  message(FATAL_ERROR "`NYANTEN_ROOT` is not defined.")
endif()
if (NOT EXISTS ${NYANTEN_ROOT})
  message(FATAL_ERROR "`NYANTEN_ROOT` does not exist.")
endif()
message("NYANTEN_ROOT=${NYANTEN_ROOT}")
include_directories(${NYANTEN_ROOT})

if (CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
  add_compile_options(-Werror)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(IS_MAJSOUL_FAIR_ENABLE_ASSERT ON)
endif()
if (IS_MAJSOUL_FAIR_ENABLE_ASSERT)
  add_compile_definitions(IS_MAJSOUL_FAIR_ENABLE_ASSERT)
endif()
message("IS_MAJSOUL_FAIR_ENABLE_ASSERT=${IS_MAJSOUL_FAIR_ENABLE_ASSERT}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    add_compile_definitions(_GLIBCXX_DEBUG _GLIBCXX_DEBUG_PEDANTIC)
  endif()
endif()

if (IS_MAJSOUL_FAIR_WITH_COVERAGE)
  add_compile_definitions(IS_MAJSOUL_FAIR_WITH_COVERAGE)
  add_compile_options("-coverage")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage")
endif()
message("IS_MAJSOUL_FAIR_WITH_COVERAGE=${IS_MAJSOUL_FAIR_WITH_COVERAGE}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    set(IS_MAJSOUL_FAIR_WITH_ASAN ON)
  endif()
endif()
if (IS_MAJSOUL_FAIR_WITH_ASAN)
  add_compile_options(-fsanitize=address)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()
message("IS_MAJSOUL_FAIR_WITH_ASAN=${IS_MAJSOUL_FAIR_WITH_ASAN}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    set(IS_MAJSOUL_FAIR_WITH_UBSAN ON)
  endif()
endif()
if (IS_MAJSOUL_FAIR_WITH_UBSAN)
  add_compile_options(-fsanitize=undefined)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=undefined")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
endif()
message("IS_MAJSOUL_FAIR_WITH_UBSAN=${IS_MAJSOUL_FAIR_WITH_UBSAN}")

if (IS_MAJSOUL_FAIR_WITH_TSAN)
  add_compile_options(-fsanitize=thread)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=thread")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
endif()
message("IS_MAJSOUL_FAIR_WITH_TSAN=${IS_MAJSOUL_FAIR_WITH_TSAN}")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_compile_options(-pthread)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

message("CMAKE_SHARED_LINKER_FLAGS=${CMAKE_SHARED_LINKER_FLAGS}")
message("CMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(Boost_USE_DEBUG_LIBS ON)
  set(Boost_USE_RELEASE_LIBS OFF)
else()
  set(Boost_USE_DEBUG_LIBS OFF)
  set(Boost_USE_RELEASE_LIBS ON)
endif()

find_package(absl REQUIRED)
find_package(Protobuf REQUIRED)

find_package(Boost_stacktrace_backtrace REQUIRED)

add_compile_definitions(BOOST_STACKTRACE_USE_BACKTRACE)

add_subdirectory(src)
