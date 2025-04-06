cmake_minimum_required(VERSION 3.18)
project(wc)

set(CMAKE_CXX_STANDARD 17)
set(ARGS_PARSER_DIR ${CMAKE_SOURCE_DIR}/../args_parser/)
set(SOURCE_FILES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_subdirectory(${ARGS_PARSER_DIR} args_parser_build)

target_link_libraries(${PROJECT_NAME} args_parser)
target_include_directories(${PROJECT_NAME} PRIVATE ${ARGS_PARSER_DIR}/include)
