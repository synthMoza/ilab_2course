cmake_minimum_required(VERSION 3.14)
project(circuit)

# Two targets - one is program itself, other is unit tests
set(CIRCUIT_RUN ${PROJECT_NAME}_run)
set(CIRCUIT_TEST ${PROJECT_NAME}_test)

# All directories and files
set(SRC_DIR src)
set(INCLUDE_DIR include)
set(PARSER_DIR parser)
set(TEST_DIR tests)
set(TEST_DET_DIR ${TEST_DIR}/determinant_tests)

set(SRC_FILES ${SRC_DIR}/graph.cpp ${SRC_DIR}/driver.cpp ${SRC_DIR}/main.cpp ${SRC_DIR}/circuit.cpp)
set(HEADERS  ${INCLUDE_DIR}/circuit.h ${INCLUDE_DIR}/graph.h ${INCLUDE_DIR}/buf.h ${INCLUDE_DIR}/driver.h ${INCLUDE_DIR}/matrix.h ${INCLUDE_DIR}/matrixbuf.h ${INCLUDE_DIR}/weight.h)
set(GTESTS ${TEST_DIR}/tests.h)
set(MAIN ${SRC_DIR}/main.cpp)

set(COMPILER ${PARSER_DIR}/compiler.y)
set(PARSER ${PARSER_DIR}/parser.l)

# Find required packages
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

# Google test definition
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e
)

enable_testing()

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Define executable files for each target
add_executable(${CIRCUIT_RUN} ${SRC_FILES} ${BISON_COMPILER_OUTPUTS} ${FLEX_PARSER_OUTPUTS})

add_executable(${CIRCUIT_TEST} ${MAIN} ${GTESTS})
target_compile_definitions(${CIRCUIT_TEST} PRIVATE DEBUG) # add debug define

# Add all tests
include(GoogleTest)
gtest_discover_tests(${CIRCUIT_TEST})

# Configure unit tests for determinant
set(TEST_DIR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DET_DIR})
configure_file(${GTESTS}.in ${GTESTS} @ONLY)

set (TARGETS ${CIRCUIT_RUN} ${CIRCUIT_TEST})
foreach (TNAME ${TARGETS})
	target_compile_features(${TNAME} PRIVATE cxx_std_17)
	target_include_directories(${TNAME} PRIVATE ${TEST_DIR})
	target_include_directories(${TNAME} PRIVATE ${INCLUDE_DIR})
	target_include_directories(${TNAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

target_link_libraries(${CIRCUIT_TEST} gtest_main)