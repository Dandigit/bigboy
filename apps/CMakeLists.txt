add_executable(bigboy-sdl
        bigboy-sdl.cpp)
target_link_libraries(bigboy-sdl PRIVATE bigboy SDL2::SDL2)

set_target_properties(bigboy-sdl PROPERTIES OUTPUT_NAME "Bigboy (SDL)")
