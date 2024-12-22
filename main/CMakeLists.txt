cmake_minimum_required(VERSION 3.00)
project(main LANGUAGES C)
set (PROGECT_NAME main)

set(CMAKE_C_CSTANDARD 99)
set(CMAKE_C_COMPILER "gcc")

file(GLOB INCLUDE_FILES ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)
set (ALL_FILES 
  ${INCLUDE_FILES}
  ${SRC_FILES}
)

add_executable(main ${ALL_FILES})
TARGET_LINK_LIBRARIES(
  main
  pthread
  utils
)

install(TARGETS main RUNTIME DESTINATION ${GLOBAL_INSTALL_DIR})