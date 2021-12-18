cmake_minimum_required(VERSION 3.14)

# Project name and a few useful settings
project(
        bigboy
        VERSION 0.1
        DESCRIPTION "A work-in-progress Gameboy emulator"
        LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_STANDARD 17)
    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    # include(CTest)

    # Docs only available if this is the main app
    # find_package(Doxygen)
    # if(Doxygen_FOUND)
    #    add_subdirectory(docs)
    # else()
    #    message(STATUS "Doxygen not found, not building docs")
    # endif()
endif()

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
# if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
#        AND BUILD_TESTING)
#    add_subdirectory(tests)
# endif()

# External library code (fetched from GitHub)
add_subdirectory(extern)

# Compiled library code
add_subdirectory(src)

# Executable code (frontends)
add_subdirectory(apps)
