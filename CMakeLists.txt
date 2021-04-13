cmake_minimum_required(VERSION 3.0)
project(rhc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -pedantic -Wno-unused-function -Wno-missing-braces")

include_directories(include)

add_executable(str examples/str.c)
