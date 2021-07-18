cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(opencl_bitonic)

# Set variables
set(SRC_DIR src)
set(INCLUDE_DIR include)

set(SRC ${SRC_DIR}/main.cpp ${SRC_DIR}/BitonicSort.cpp)
set(HEADERS ${INCLUDE_DIR}/BitonicSort.h)

# Find OpenCL package
find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIR})
include_directories(${INCLUDE_DIR})

# Add executable, link OpenCL library
add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${OpenCL_LIBRARIES})