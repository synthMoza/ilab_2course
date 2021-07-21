set(BINARY ${PROJECT_NAME}_test)

set(TEST_LIST tests.cpp)
set(TEST_SRC main.cpp)
set(TEST_INPUT_DIR Tests)

# Configute path to the test directory
set(TEST_DIR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_INPUT_DIR})
configure_file(${TEST_LIST}.in ${TEST_LIST} @ONLY)

# Configure source and include files
add_executable(${BINARY} ${TEST_SRC} ${CMAKE_CURRENT_BINARY_DIR}/${TEST_LIST})
add_test(NAME ${BINARY} COMMAND ${BINARY})
target_link_libraries(${BINARY} PUBLIC ${PROJECT_NAME}_lib gtest)