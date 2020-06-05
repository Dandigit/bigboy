#ifndef BIGBOY_JOYPAD_H
#define BIGBOY_JOYPAD_H

#include <bigboy/MMU/MemoryDevice.h>

class Joypad : public MemoryDevice {
public:
    std::vector<AddressSpace> addressSpaces() const;
    uint8_t readByte(uint16_t address) const = 0;
    void writeByte(uint16_t address, uint8_t value) = 0;
};

#endif //BIGBOY_JOYPAD_H
