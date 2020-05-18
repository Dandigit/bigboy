#ifndef BIGBOY_CARTRIDGE_H
#define BIGBOY_CARTRIDGE_H

#include <array>

#include <bigboy/MMU/MemoryDevice.h>

class Cartridge : public MemoryDevice {
    std::array<uint8_t, 0x3FFF + 1> m_bank0{0};
    std::array<uint8_t, 0x3FFF + 1> m_bank1{0};

    std::array<uint8_t, 0x1FFF + 1> m_extRAM{0};

public:
    Cartridge() = default;
    Cartridge(std::array<uint8_t, 0x3FFF + 1> bank0, std::array<uint8_t, 0x3FFF + 1> bank1);
    ~Cartridge() = default;

    static Cartridge fromFile(std::string fileName);

    template <typename... T>
    static Cartridge test(T... contents);

    std::vector<AddressSpace> addressSpaces() const override;
    uint8_t readByte(uint16_t address) const override;
    void writeByte(uint16_t address, uint8_t value) override;
};

template<typename... T>
Cartridge Cartridge::test(T... contents) {
    std::array<uint8_t, 0x3FFF + 1> bank0{0};
    std::array<uint8_t, 0x3FFF + 1> bank1{0};

    const std::vector<uint8_t> bytes{static_cast<uint8_t>(contents)...};
    for (size_t i = 0; i < bytes.size(); ++i) {
        bank0[i + 0x100] = bytes[i];
    }

    return Cartridge{std::move(bank0), std::move(bank1)};
}

#endif //BIGBOY_CARTRIDGE_H
