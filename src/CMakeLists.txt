include(TestBigEndian)
test_big_endian(BIGBOY_BIG_ENDIAN)
if(${BIGBOY_BIG_ENDIAN})
    add_compile_definitions(BIGBOY_BIG_ENDIAN)
endif()

add_compile_definitions(BIGBOY_SCREEN_TINT)

add_library(bigboy
        APU.cpp
        ../include/bigboy/APU.h
        Cartridge.cpp
        ../include/bigboy/Cartridge.h
        CartridgeHeader.cpp
        ../include/bigboy/CartridgeHeader.h
        CPU.cpp
        ../include/bigboy/CPU.h
        Emulator.cpp
        ../include/bigboy/Emulator.h
        GPU.cpp
        ../include/bigboy/GPU.h
        InternalMemory.cpp
        ../include/bigboy/InternalMemory.h
        Joypad.cpp
        ../include/bigboy/Joypad.h
        ../include/bigboy/MemoryDevice.h
        MMU.cpp
        ../include/bigboy/MMU.h
        ../include/bigboy/OpCode.h
        ../include/bigboy/PrefixOpCode.h
        Registers.cpp
        ../include/bigboy/Registers.h
        Serial.cpp
        ../include/bigboy/Serial.h
        Timer.cpp
        ../include/bigboy/Timer.h)
target_include_directories(bigboy PUBLIC ../include)