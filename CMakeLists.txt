cmake_minimum_required(VERSION 3.12)

project(OSM2GeoJSON VERSION 0.1
    DESCRIPTION "Program to handle openstreetmap datas and retrieve informations about ecological landscapes"
    LANGUAGES CXX)

# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

#################### Options ####################
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CONAN_DISABLE_CHECK_COMPILER ON)

option(WARNINGS "" OFF)
option(COMPILE_FOR_NATIVE "" OFF)
option(COMPILE_WITH_LTO "" OFF)

set(WARNINGS_LIST "-Wall;-Wextra;")

# ################### Modules ####################
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(CMakeBuildTypeUtils)

#################### Packages ####################
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(Osmium REQUIRED)
find_package(Protozero REQUIRED)


add_executable(osm2geojson src/io/parse_patterns.cpp
                        src/io/print_geojson.cpp
                        src/io/print_svg_result.cpp
                        src/query_osm_file.cpp
                        src/osm2geojson.cpp)
target_include_directories(osm2geojson PRIVATE include)
target_link_libraries(osm2geojson CONAN_PKG::boost)
target_link_libraries(osm2geojson CONAN_PKG::simdjson)
target_link_libraries(osm2geojson CONAN_PKG::nlohmann_json)
target_link_libraries(osm2geojson CONAN_PKG::expat)
target_link_libraries(osm2geojson CONAN_PKG::zlib)
target_include_directories(osm2geojson PUBLIC ${OSMIUM_INCLUDE_DIR})
target_include_directories(osm2geojson PUBLIC ${PROTOZERO_INCLUDE_DIR})
target_compile_options_if_exists(osm2geojson PRIVATE ${WARNINGS} ${WARNINGS_LIST})
target_compile_options_if_exists(osm2geojson PRIVATE ${COMPILE_FOR_NATIVE} -march=native)
target_compile_options_if_exists(osm2geojson PRIVATE ${COMPILE_WITH_LTO} -flto)
