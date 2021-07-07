add_library(bigboy
        ${CMAKE_CURRENT_SOURCE_DIR}/APU.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Cartridge.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CartridgeHeader.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/CPU.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Emulator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Registers.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/GPU.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Joypad.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/InternalMemory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MMU.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Serial.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Timer.cpp)