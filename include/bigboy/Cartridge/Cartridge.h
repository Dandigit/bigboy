#ifndef BIGBOY_CARTRIDGE_H
#define BIGBOY_CARTRIDGE_H

#include <array>

#include <bigboy/MMU/MemoryDevice.h>

// 0147: Cartridge Type
// Specifies which Memory Bank Controller (if any) is used in
// the cartridge, and if further external hardware exists
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

// 0148: ROM Size
// Specifies the ROM Size of the cartridge. Typically calculated as
// "32KB << N".
enum class ROMSize {
    KB_32  = 0x00, // No ROM banking
    KB_64  = 0x01, // 4 banks
    KB_128 = 0x02, // 8 banks
    KB_256 = 0x03, // 16 banks
    KB_512 = 0x04, // 32 banks
    MB_1   = 0x05, // 64 banks
    MB_2   = 0x06, // 128 banks
    MB_4   = 0x07, // 256 banks
    MB_1_1 = 0x52, // 72 banks
    MB_1_2 = 0x53, // 80 banks
    MB_1_5 = 0x54, // 96 banks
};

// 0149: RAM Size
// Specifies the size of the external RAM in the cartridge (if any).
enum class RAMSize {
    NONE  = 0x00, // No RAM
    KB_2  = 0x01, // 1x2KB bank
    KB_8  = 0x02, // 1x8KB bank
    KB_32 = 0x03, // 4x8KB banks
};

// 014A: Destination Code
// Specifies if this version of the game is supposed to be sold in japan,
// or anywhere else. Only two values are defined.
enum class DestinationCode {
    JAPAN     = 0x00,
    NOT_JAPAN = 0x01,
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
    ROMSize getROMSize() const;
    uint32_t getROMSizeInBytes() const; // Excludes bank 0
    RAMSize getRAMSize() const;
    uint32_t getRAMSizeInBytes() const; // Includes bank 0
    DestinationCode getDestinationCode() const;

    bool isRAMEnabled() const;
    uint8_t getROMBankNumber() const;

    // 0000-3FFF: 16KB ROM Bank 00 (in cartridge, fixed at bank 00)
    std::array<uint8_t, 0x3FFF + 1> m_bank0{0};

    // 4000-7FFF: 16KB ROM Bank 01..NN (in cartridge, switchable bank number)
    std::vector<uint8_t> m_bank1Onwards{};

    // A000-BFFF: 8KB External RAM (in cartridge, switchable bank, if any)
    std::vector<uint8_t> m_extRAM{};

    // 0000-1FFF: RAM Enable
    // Write-only (lowest 4 bits)
    //  00: Disable RAM (default)
    //  0A: Enable RAM
    uint8_t m_ramEnable;

    // 2000-3FFF: Current ROM switchable bank number
    // Write-only (lowest 5 bits)
    // Between 01 and 1F
    uint8_t m_romBankSelect;

    // 4000-5FFF: Current RAM switchable bank number or upper 2 bits
    // of ROM bank number (depending on banking mode)
    // Write-only (lowest 2 bits)
    uint8_t m_ramBankSelect;

    // 6000-7FFF: Current banking mode
    // Write-only (lowest 1 bit)
    //  0: ROM Banking Mode (up to 8KB RAM, 2MB ROM)
    //  1: RAM Banking Mode (up to 32KB RAM, 512KB ROM)
    uint8_t m_bankingModeSelect;
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

class CartridgeLoadError : public std::runtime_error {
public:
    explicit CartridgeLoadError(const std::string& what) :
            std::runtime_error{what} {
    }
};

#endif //BIGBOY_CARTRIDGE_H
