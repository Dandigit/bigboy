#ifndef BIGBOY_CARTRIDGE_H
#define BIGBOY_CARTRIDGE_H

#include <array>

#include <bigboy/MMU/MemoryDevice.h>

enum class MBCType : uint8_t {
    ROM                     = 0x00,
    MBC1                    = 0x01,
    MBC1_RAM                = 0x02,
    MBC1_RAM_BAATTERY       = 0x03,
    MBC2                    = 0x05,
    MBC2_BATTERY            = 0x06,
    ROM_RAM                 = 0x08,
    ROM_RAM_BATTERY         = 0x09,
    MMM01                   = 0x0B,
    MMM01_RAM               = 0x0C,
    MMM01_RAM_BATTERY       = 0x0D,
    MBC3_TIMER_BATTERY      = 0x0F,
    MBC3_TIMER_RAM_BATTERY  = 0x10,
    MBC3                    = 0x11,
    MBC3_RAM                = 0x12,
    MBC3_RAM_BATTERY        = 0x13,
    MBC4                    = 0x15,
    MBC4_RAM                = 0x16,
    MBC4_RAM_BATTERY        = 0x17,
    MBC5                    = 0x19,
    MBC5_RAM                = 0x1A,
    MBC5_RAM_BATTERY        = 0x1B,
    MBC5_RUMBLE             = 0x1C,
    MBC5_RUMBLE_RAM         = 0x1D,
    MBC5_RUMBLE_RAM_BATTERY = 0x1E,
    POCKET_CAMERA           = 0xFC,
    BANDAI_TAMA5            = 0xFD,
    HUC3                    = 0xFE,
    HUC1_RAM_BATTERY        = 0xFF,
};

class Cartridge : public MemoryDevice {
public:
    Cartridge() = default;
    Cartridge(const std::vector<uint8_t>& bytes);
    ~Cartridge();

    static Cartridge fromFile(std::string fileName);

    template <typename... T>
    static Cartridge test(T... contents);

    std::vector<AddressSpace> addressSpaces() const override;
    uint8_t readByte(uint16_t address) const override;
    void writeByte(uint16_t address, uint8_t value) override;

    std::string getGameTitle() const;

private:
    MBCType getMBCType() const;

    // 0000-3FFF: 16KB ROM Bank 00 (in cartridge, fixed at bank 00)
    std::array<uint8_t, 0x3FFF + 1> m_bank0{0};

    // 4000-7FFF: 16KB ROM Bank 01..NN (in cartridge, switchable bank number)
    std::vector<uint8_t> m_bank1Onwards{};

    // A000-BFFF: 8KB External RAM (in cartridge, switchable bank, if any)
    std::vector<uint8_t> m_extRAM{};
};

template<typename... T>
Cartridge Cartridge::test(T... contents) {
    std::array<uint8_t, 0x3FFF + 1> bank0{0};
    std::array<uint8_t, 0x3FFF + 1> bank1{0};

    const std::vector<uint8_t> bytes{static_cast<uint8_t>(contents)...};
    for (size_t i = 0; i < bytes.size(); ++i) {
        bank0[i + 0x100] = bytes[i];
    }

    return Cartridge{bytes};
}

#endif //BIGBOY_CARTRIDGE_H
