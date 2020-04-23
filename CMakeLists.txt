cmake_minimum_required(VERSION 3.15)
project(bigboy)
set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_subdirectory(extern)

add_subdirectory(lib)
add_subdirectory(src)

add_subdirectory(tests)