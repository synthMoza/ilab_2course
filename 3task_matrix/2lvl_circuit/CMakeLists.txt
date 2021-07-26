cmake_minimum_required(VERSION 3.14)
project(circuit)

set(CMAKE_CXX_STANDART 11)

set(SRC_DIR src)
set(INCLUDE_DIR include)
set(PARSER_DIR parser)

set(SRC_FILES ${SRC_DIR}/driver.cpp ${SRC_DIR}/main.cpp)
set(HEADERS  ${INCLUDE_DIR}/graph.h ${INCLUDE_DIR}/buf.h ${INCLUDE_DIR}/driver.h ${INCLUDE_DIR}/matrix.h ${INCLUDE_DIR}/matrixbuf.h ${INCLUDE_DIR}/weight.h)

set(COMPILER ${PARSER_DIR}/compiler.y)
set(PARSER ${PARSER_DIR}/parser.l)

find_package(BISON 3.6)
if (BISON_FOUND)
	BISON_TARGET(COMPILER ${COMPILER} ${CMAKE_CURRENT_BINARY_DIR}/compiler.cpp)
else()
	message(FATAL_ERROR "Can't find bison package!")
endif()

find_package(FLEX 2.6)
if (FLEX_FOUND)
	FLEX_TARGET(PARSER ${PARSER} ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
else()
	message(FATAL_ERROR "Can't find flex package!")
endif()

ADD_FLEX_BISON_DEPENDENCY(PARSER COMPILER)

include_directories(${INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(${PROJECT_NAME} ${SRC_FILES} ${BISON_COMPILER_OUTPUTS} ${FLEX_PARSER_OUTPUTS})