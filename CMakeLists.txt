cmake_minimum_required(VERSION 3.6)
project(CppLenses)

set(CMAKE_CXX_STANDARD 14)

add_library(cpp_lens INTERFACE)
target_include_directories(cpp_lens INTERFACE "include")

add_subdirectory(tests)
add_subdirectory(examples)