add_library(bigboy
        ${CMAKE_CURRENT_SOURCE_DIR}/Cartridge/Cartridge.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Cartridge/CartridgeHeader.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/CPU/CPU.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/CPU/Registers.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/GPU/GPU.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Joypad.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/MMU/InternalMemory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MMU/MMU.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Serial.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/Timer.cpp)