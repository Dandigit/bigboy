#ifndef BIGBOY_SERIAL_H
#define BIGBOY_SERIAL_H

#include <bigboy/MemoryDevice.h>

class Serial : public MemoryDevice {
public:
    Serial() = default;

    std::vector<AddressSpace> addressSpaces() const override;
    uint8_t readByte(uint16_t address) const override;
    void writeByte(uint16_t address, uint8_t value) override;

private:
    uint8_t m_data = 0;    // FF01: Serial data (SB)
    uint8_t m_control = 0; // FF02: Serial control (SC)
};

#endif //BIGBOY_SERIAL_H
