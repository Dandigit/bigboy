#ifndef BIGBOY_MEMORYDEVICE_H
#define BIGBOY_MEMORYDEVICE_H

#include <cstdint>

// A device that owns some section(s)? of memory
class MemoryDevice {
public:
    virtual bool doesAddress(uint16_t address) const = 0;
    virtual uint8_t readByte(uint16_t address) const = 0;
    virtual void writeByte(uint16_t address, uint8_t value) = 0;
};

#endif //BIGBOY_MEMORYDEVICE_H
