cmake_minimum_required(VERSION 3.00)
project(test_upd_tcp C)
set(CMAKE_C_CSTANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
include_directories(HEADER_FILES)
set(SRC
    "./main.c"
    )
add_executable(exec ${SRC})