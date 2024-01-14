cmake_minimum_required(VERSION 3.27)
project(streval)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_library(streval STATIC
        src/identifier.cpp
        src/scope.cpp
        src/streval.cpp
        src/streval_utils.cpp
        src/oper.cpp
)

add_executable(streval_tests tests/eval_test.cpp
        tests/testcase.h
)

target_link_libraries(streval_tests streval)

enable_testing()

add_test(
        NAME streval
        COMMAND streval_tests
)