add_executable(bigboy-bin
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui-SFML.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp)

find_package(OpenGL REQUIRED)

target_link_libraries(bigboy-bin PRIVATE bigboy sfml-system sfml-window sfml-graphics OpenGL::GL)
target_include_directories(bigboy-bin PRIVATE SFML_INCLUDE_DIR)
set_target_properties(bigboy-bin PROPERTIES OUTPUT_NAME Bigboy)