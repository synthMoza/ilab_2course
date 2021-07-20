cmake_minimum_required(VERSION 2.6)

project(circuit)

set(SRC_DIR src)
set(INCLUDE_DIR include)

set(SRC ${SRC_DIR}/main.cpp)
set(HEADERS ${INCLUDE_DIR}/buf.h ${INCLUDE_DIR}/matrix.h ${INCLUDE_DIR}/matrixbuf.h)

include_directories(${INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${SRC})