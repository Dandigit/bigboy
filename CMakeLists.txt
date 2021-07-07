cmake_minimum_required(VERSION 3.15)
project(bigboy)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(extern/SFML)

add_library(bigboy
        lib/APU.cpp
        lib/Cartridge.cpp
        lib/CartridgeHeader.cpp
        lib/CPU.cpp
        lib/Emulator.cpp
        lib/GPU.cpp
        lib/InternalMemory.cpp
        lib/Joypad.cpp
        lib/MMU.cpp
        lib/Registers.cpp
        lib/Serial.cpp
        lib/Timer.cpp)
target_include_directories(bigboy PUBLIC include)

add_executable(bigboy-bin
        src/main.cpp)
target_link_libraries(bigboy-bin PRIVATE bigboy sfml-system sfml-window sfml-graphics)
set_target_properties(bigboy-bin PROPERTIES OUTPUT_NAME Bigboy)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

include(TestBigEndian)
test_big_endian(BIGBOY_BIG_ENDIAN)
if(${BIGBOY_BIG_ENDIAN})
    add_compile_definitions(BIGBOY_BIG_ENDIAN)
endif()

add_compile_definitions(BIGBOY_SCREEN_TINT)
