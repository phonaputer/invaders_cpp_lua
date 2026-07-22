cmake_minimum_required(VERSION 3.28.0)
project(game VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(tests)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

if(EMSCRIPTEN)
  target_link_options(${PROJECT_NAME} PRIVATE "SHELL:--embed-file ${CMAKE_BINARY_DIR}/assets@/assets")
endif()