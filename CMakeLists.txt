cmake_minimum_required(VERSION 3.12)

project(NdsTile VERSION 0.1
  DESCRIPTION "Program to handle convert WGS84 to Nds Tile"
  LANGUAGES CXX)

# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

# ################### Options ####################
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CONAN_DISABLE_CHECK_COMPILER ON)

option(WARNINGS "" OFF)
option(COMPILE_FOR_NATIVE "" OFF)
option(COMPILE_WITH_LTO "" OFF)

set(WARNINGS_LIST "-Wall;-Wextra;")

# glog
find_package(Glog REQUIRED)
include_directories(BEFORE ${GLOG_INCLUDE_DIRS})

# gtest
find_package(GTest REQUIRED)

include_directories(include)
file(GLOB_RECURSE SRC src/*.cc)
add_library(nds_tiles_converter ${SRC})
target_link_libraries(nds_tiles_converter glog gflags)

add_executable(nds_coordinate_test test/nds_coordinate_test.cc)
target_link_libraries(nds_coordinate_test nds_tiles_converter gtest)
add_executable(nds_tile_test test/nds_tile_test.cc)
target_link_libraries(nds_tile_test nds_tiles_converter gtest)
