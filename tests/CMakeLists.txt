link_directories("../gtest")
include_directories("../gtest/include")

add_executable(
        cpp_lens_test
        tests.cpp)

target_link_libraries(
        cpp_lens_test
        gtest
        cpp_lens)