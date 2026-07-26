add_library(${PROJECT_NAME}_lib)
target_link_libraries(
  ${PROJECT_NAME}_lib PUBLIC 
    compiler_flags vendor_lib
)
target_include_directories(
  ${PROJECT_NAME}_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(framework)
add_subdirectory(game)

add_executable(
  ${PROJECT_NAME} 
    main.cpp
)
target_link_libraries(
  ${PROJECT_NAME} PRIVATE 
    ${PROJECT_NAME}_lib
)

set_target_properties(
  ${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)