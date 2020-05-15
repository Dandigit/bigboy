add_library(bigboy
        ${CMAKE_CURRENT_SOURCE_DIR}/CPU/CPU.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CPU/Flags.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CPU/Registers.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/MMU/InternalMemory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MMU/MMU.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Serial.cpp)