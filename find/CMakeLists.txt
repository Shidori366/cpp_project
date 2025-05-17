cmake_minimum_required(VERSION 3.18)
project(find)

set(CMAKE_CXX_STANDARD 20)
set(ARGS_PARSER_DIR ${CMAKE_SOURCE_DIR}/../args_parser)

add_compile_options(-g)

add_executable(find main.cpp
        find.cpp
        find.h)

add_subdirectory(${ARGS_PARSER_DIR} args_parser_build)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${ARGS_PARSER_DIR}/include)
target_link_libraries(${CMAKE_PROJECT_NAME} args_parser)
