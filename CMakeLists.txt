cmake_minimum_required(VERSION 3.00)

project(test_upd_tcp LANGUAGES C)
set(CMAKE_C_CSTANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

add_subdirectory(utils)
add_subdirectory(main)
