# code copied from https://crascit.com/2015/07/25/cmake-gtest/
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(benchmark-download NONE)

include(ExternalProject)

ExternalProject_Add(
  benchmark
  SOURCE_DIR "@BENCHMARK_DOWNLOAD_ROOT@/benchmark-src"
  BINARY_DIR "@BENCHMARK_DOWNLOAD_ROOT@/benchmark-build"
  GIT_REPOSITORY
    https://github.com/google/benchmark.git
  GIT_TAG
    v1.5.2
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  TEST_COMMAND ""
  )
