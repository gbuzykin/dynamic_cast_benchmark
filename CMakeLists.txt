cmake_minimum_required(VERSION 3.21)

project(
  uxs
  DESCRIPTION "dynamic_cast_benchmark"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD
    20
    CACHE STRING "Default C++ standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("Using C++${CMAKE_CXX_STANDARD} standard")

add_executable(dynamic_cast_benchmark dynamic_cast_benchmark.cpp
                                      Priori/src/priori.cpp)

target_compile_options(dynamic_cast_benchmark
                       PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus /utf-8>)

target_include_directories(dynamic_cast_benchmark PRIVATE Priori/include
                                                          kcl/Source)
