cmake_minimum_required(VERSION 3.18)
project(split)

set(CMAKE_CXX_STANDARD 17)
set(ARGS_PARSER_DIR ${CMAKE_SOURCE_DIR}/../args_parser)

add_executable(${CMAKE_PROJECT_NAME} main.cpp
        split.cpp
        split.h)

add_subdirectory(${ARGS_PARSER_DIR} args_parser_build)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${ARGS_PARSER_DIR}/include)
target_link_libraries(${CMAKE_PROJECT_NAME} args_parser)
