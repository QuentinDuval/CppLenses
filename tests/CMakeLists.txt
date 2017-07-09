link_directories("../gtest")
include_directories("../gtest/include")

add_executable(
        cpp_lens_test
        tests.cpp
        lens_tests.cpp
        billing_account.cpp
        billing_account.h)

target_link_libraries(
        cpp_lens_test
        gtest
        cpp_lens)