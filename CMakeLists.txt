cmake_minimum_required(VERSION 4.1)
project(splendor CXX)

include_directories("include")
add_library(contents src/contents.cpp)
# add_library(game include/game.hpp)

add_executable(main src/main.cpp)
target_link_libraries(main contents)
