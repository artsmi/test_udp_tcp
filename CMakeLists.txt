cmake_minimum_required(VERSION 3.00)

project(test_upd_tcp LANGUAGES C)
set(CMAKE_C_CSTANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

add_subdirectory(utils)

include_directories(HEADER_FILES)
set(SRC
  "./main.c"
)

add_executable(exec ${SRC})
TARGET_LINK_LIBRARIES(
  exec
  pthread
  utils
)

install(TARGETS exec RUNTIME DESTINATION ${GLOBAL_INSTALL_DIR})
