add_executable(bigboy-bin
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_link_libraries(bigboy-bin PRIVATE bigboy sfml-system sfml-window sfml-graphics)
target_include_directories(bigboy-bin PRIVATE SFML_INCLUDE_DIR)
set_target_properties(bigboy-bin PROPERTIES OUTPUT_NAME Bigboy)