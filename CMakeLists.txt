cmake_minimum_required(VERSION 2.8)
project(ConfigParser)

set(CMAKE_C_FLAGS_DEBUG "-Wall -pedantic -Wno-unused-but-set-variable -g -O0 -std=c99")
set(CMAKE_C_FLAGS_RELEASE "-Wall -pedantic -Wno-unused-but-set-variable -O2 -std=c99")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)W
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_INCLUDE_CURRENT_DIR ${CMAKE_SOURCE_DIR}/include/)

file(GLOB_RECURSE ALL_SROUCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(config_parser src/parser.c)

enable_testing()

add_executable(test_config_parser test/test_config_parser.c)
target_link_libraries(test_config_parser config_parser)
add_test(NAME config_parser_test
    COMMAND ./test_config_parser ../test/demo.conf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

