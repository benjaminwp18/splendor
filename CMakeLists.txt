cmake_minimum_required(VERSION 4.1)
project(splendor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Generate compile_commands.json static analyzers

# Small project -> single lib
add_library(splendor STATIC
    src/cards.cpp
    src/money.cpp
    src/game.cpp
    src/player.cpp
)

target_include_directories(splendor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE splendor)
