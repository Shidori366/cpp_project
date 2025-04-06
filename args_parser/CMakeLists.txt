cmake_minimum_required(VERSION 3.18)
project(args_parser)

set(CMAKE_CXX_STANDARD 17)

set(LIB_SOURCE_FILES
        src/args_parser.cpp
        src/exceptions/value_is_missing_exception.cpp
        src/exceptions/required_option_is_missing_exception.cpp
        include/args_parser.h
        include/option_descriptor.h
        include/exceptions/value_is_missing_exception.h
        include/exceptions/required_option_is_missing_exception.h)

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)