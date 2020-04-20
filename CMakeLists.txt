cmake_minimum_required(VERSION 3.15)
project(bigboy)
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        2.5.1
)
FetchContent_MakeAvailable(sfml)

FetchContent_GetProperties(sfml)

if (NOT sfml_POPULATED)
    message(STATUS "SFML source dir: ${sfml_SOURCE_DIR}")
    message(STATUS "SFML binary dir: ${sfml_BINARY_DIR}")
    FetchContent_Populate(sfml)

    set(SFML_INCLUDE_DIR "${sfml_SOURCE_DIR}/include")
    set(SFML_LIBRARY_DIR "${sfml_BINARY_DIR}/lib")

    link_directories(SFML_LIBRARY_DIR)
    include_directories(SFML_INCLUDE_DIR)

    find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)
endif()

add_custom_target(sfml-graphics-freetype-symlink-fix ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink "${sfml_SOURCE_DIR}/extlibs/headers/freetype2" "${sfml_SOURCE_DIR}/extlibs/headers/freetype2/freetype")
add_dependencies(sfml-graphics sfml-graphics-freetype-symlink-fix)

add_executable(bigboy src/main.cpp src/CPU.h src/CPU.cpp)
target_link_libraries(bigboy sfml-system sfml-window sfml-graphics)