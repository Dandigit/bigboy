# FetchContent downloads during the configure step
include(FetchContent)

# SDL
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.0.18)
FetchContent_MakeAvailable(SDL2)


