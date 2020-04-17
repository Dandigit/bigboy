cmake_minimum_required(VERSION 3.15)
project(bigboy)

set(CMAKE_CXX_STANDARD 17)

add_executable(bigboy src/main.cpp src/CPU.h src/CPU.cpp)