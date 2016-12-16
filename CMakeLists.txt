cmake_minimum_required(VERSION 2.8.12)
include(CMakeToolsHelpers)
enable_testing()

project(Phoenix)

include_directories(lib)
include_directories(src)

set(PHOENIX_TEST
    test/val.cpp
    test/ref.cpp
    test/arg.cpp
    test/function.cpp
)

set(PHOENIX_SRC
    src/phoenix.hpp
)

add_executable(phoenix-test
    ${PHOENIX_TEST}
    ${PHOENIX_SRC}
)

set_property(TARGET phoenix-test PROPERTY CXX_STANDARD 98)

add_test(phoenix-tests phoenix-test)
