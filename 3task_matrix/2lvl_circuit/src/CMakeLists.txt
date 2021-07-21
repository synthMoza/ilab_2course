set(BINARY ${PROJECT_NAME})

set(SRC main.cpp)

add_executable(${BINARY}_run ${SRC})
add_library(${BINARY}_lib STATIC ${SRC})