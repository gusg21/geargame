cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(geargame_test)

include(cmake/get_cpm.cmake)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Our Project
add_executable(${PROJECT_NAME} 

    src/main.c
    
    )

# Libraries
# NOTE: This project should NOT require raylib
add_subdirectory(src/gg)
target_link_libraries(${PROJECT_NAME} gglib)

# Copy Assets
if (APPLE)
    add_custom_target(copy_assets 
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
    )
    add_dependencies(${PROJECT_NAME} copy_assets)
endif()

if (WIN32)
    add_custom_target(copy_assets 
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/Debug/assets
    )
    add_dependencies(${PROJECT_NAME} copy_assets)
endif()

# Flags
target_compile_definitions(${PROJECT_NAME} PUBLIC PROJECT_NAME=${PROJECT_NAME})

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()