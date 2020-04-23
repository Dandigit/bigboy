# Googletest
add_subdirectory(googletest)

# SFML
add_subdirectory(SFML)

#set(SFML_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML/build/lib")
#set(SFML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML/include")

#link_directories(SFML_BINARY_DIR)
#include_directories(SFML_INCLUDE_DIR)

add_custom_target(sfml-graphics-freetype-symlink-fix ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/SFML/extlibs/headers/freetype2"
        "${CMAKE_CURRENT_SOURCE_DIR}/SFML/extlibs/headers/freetype2/freetype")
add_dependencies(sfml-graphics sfml-graphics-freetype-symlink-fix)