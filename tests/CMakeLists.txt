add_executable(bigboy-tests ${CMAKE_CURRENT_SOURCE_DIR}/src/CPUTests.cpp)

target_link_libraries(bigboy-tests PRIVATE bigboy gtest_main)
target_include_directories(bigboy-tests PRIVATE GOOGLETEST_INCLUDE_DIR)