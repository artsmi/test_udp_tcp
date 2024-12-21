cmake_minimum_required(VERSION 3.00)
project(utils LANGUAGES C)
set (PROGECT_NAME utils)

set(CMAKE_C_CSTANDARD 99)
set(CMAKE_C_COMPILER "gcc")

file(GLOB INCLUDE_FILES ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)
set (ALL_FILES 
  ${INCLUDE_FILES}
  ${SRC_FILES}
)

add_library(utils STATIC ${ALL_FILES})
target_include_directories(utils
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
set_target_properties(utils
  PROPERTIES PUBLIC_HEADER 
    "${INCLUDE_FILES}"
)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${PROGECT_NAME}.a
  DESTINATION ${GLOBAL_INSTALL_DIR}/${PROGECT_NAME}
)
install (FILES ${INCLUDE_FILES}
  DESTINATION ${GLOBAL_INSTALL_DIR}/${PROGECT_NAME}/include
)
