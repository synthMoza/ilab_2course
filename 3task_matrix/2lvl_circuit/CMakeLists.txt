cmake_minimum_required(VERSION 3.10)
project(circuit)

set(CMAKE_CXX_STANDART 11)

set(SRC_DIR src)
set(INCLUDE_DIR include)
set(TEST_DIR test)
set(GTEST_DIR lib/googletest)

include_directories(${INCLUDE_DIR})

add_subdirectory(${SRC_DIR})
add_subdirectory(${TEST_DIR})
add_subdirectory(${GTEST_DIR})