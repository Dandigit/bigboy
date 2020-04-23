cmake_minimum_required(VERSION 3.15)
project(bigboy)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(extern)

add_executable(bigboy src/main.cpp src/CPU.h src/CPU.cpp)
target_link_libraries(bigboy sfml-system sfml-window sfml-graphics)