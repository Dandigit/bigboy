add_executable(bigboy-tests ${CMAKE_CURRENT_SOURCE_DIR}/src/CPUTests.cpp)
target_link_libraries(bigboy-tests libbigboy gtest_main)