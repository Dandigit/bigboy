add_executable(bigboy-sdl2
        main.cpp)
target_link_libraries(bigboy-sdl2 PRIVATE bigboy SDL2::SDL2main)

set_target_properties(bigboy-sdl2 PROPERTIES OUTPUT_NAME "Bigboy (SDL2)")
