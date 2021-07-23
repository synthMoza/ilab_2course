cmake_minimum_required(VERSION 3.14)

project(paracl_compiler)

# Declare variables
set(INCLUDE_DIR include)
set(SRC_DIR src)

set(SRC_FILES ${SRC_DIR}/driver.cc ${SRC_DIR}/main.cc ${SRC_DIR}/Node.cc ${SRC_DIR}/Scanner.cc ${SRC_DIR}/Symtab.cc)
set(HEADERS ${INCLUDE_DIR}/driver.hpp ${INCLUDE_DIR}/Node.hpp ${INCLUDE_DIR}/Scanner.hpp ${INCLUDE_DIR}/Symtab.hpp)

set(COMPILER compiler/compiler.y)
set(SCANNER compiler/scanner.l)

# Configure compiler and scanner
find_package(BISON 3.6)
if (BISON_FOUND)
	BISON_TARGET(COMPILER ${COMPILER} ${CMAKE_CURRENT_BINARY_DIR}/compiler.cc 
		COMPILE_FLAGS -Wcounterexamples)
else()
	message(FATAL_ERROR "Can't find bison package!")
endif()

find_package(FLEX 2.6)
if (FLEX_FOUND)
	FLEX_TARGET(SCANNER ${SCANNER} ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)
else()
	message(FATAL_ERROR "Can't find flex package!")
endif()

ADD_FLEX_BISON_DEPENDENCY(SCANNER COMPILER)

# Add headers to include path
include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# Add all source files
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADERS} ${BISON_COMPILER_OUTPUTS} ${FLEX_SCANNER_OUTPUTS})