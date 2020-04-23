cmake_minimum_required(VERSION 3.15)
project(bigboy)
set(CMAKE_CXX_STANDARD 14)
enable_testing()

add_subdirectory(extern)

add_library(libbigboy src/CPU.h src/CPU.cpp)

add_executable(bigboy src/main.cpp)
target_link_libraries(bigboy libbigboysfml-system sfml-window sfml-graphics)

add_subdirectory(tests)