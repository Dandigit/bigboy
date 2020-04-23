# Googletest
add_subdirectory(googletest)
set(GOOGLETEST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/googletest/include" PARENT_SCOPE)

# SFML
add_subdirectory(SFML)
set(SFML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML/include" PARENT_SCOPE)

add_custom_target(sfml-graphics-freetype-symlink-fix ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/SFML/extlibs/headers/freetype2"
        "${CMAKE_CURRENT_SOURCE_DIR}/SFML/extlibs/headers/freetype2/freetype")
add_dependencies(sfml-graphics sfml-graphics-freetype-symlink-fix)