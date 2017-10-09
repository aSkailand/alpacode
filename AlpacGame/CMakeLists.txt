cmake_minimum_required(VERSION 3.8)
project(AlpacGame)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)
add_executable(AlpacGame ${SOURCE_FILES})
target_link_libraries(AlpacGame sfml-graphics sfml-window sfml-system)
target_include_directories(AlpacGame PUBLIC plog/include)