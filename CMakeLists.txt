cmake_minimum_required(VERSION 3.20)

project(pacman LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# SFML
add_subdirectory("lib/SFML")

# Tile level library (from previous lab)
add_library(tile_level STATIC
    tile_level_loader/level_system.cpp
)

target_include_directories(tile_level PUBLIC
    tile_level_loader
)

target_link_libraries(tile_level PUBLIC
    sfml-graphics
)

# New engine library
add_library(engine STATIC
    engine/game_system.cpp
    engine/entity.cpp
    engine/renderer.cpp
)

target_include_directories(engine PUBLIC
    engine
)

target_link_libraries(engine PUBLIC
    sfml-graphics
)

# Resources
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "${CMAKE_BINARY_DIR}/bin/assets"
)

# Pacman game
add_executable(pacman
    main.cpp
    scenes.cpp
    components.cpp
)

target_link_libraries(pacman PRIVATE
    engine
    tile_level
    sfml-graphics
)

set_target_properties(pacman PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

add_dependencies(pacman copy_resources)