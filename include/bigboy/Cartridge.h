#ifndef BIGBOY_CARTRIDGE_H
#define BIGBOY_CARTRIDGE_H

#include <array>
#include <memory>
#include <chrono>

#include "CartridgeHeader.h"
#include "MemoryDevice.h"

class Cartridge : public MemoryDevice {
public:
    Cartridge(std::vector<uint8_t> rom, std::vector<uint8_t> ram, CartridgeHeader header);
    virtual ~Cartridge() = default;

    std::vector<AddressSpace> addressSpaces() const override;

    bool loadRamFileIfSupported(const std::string& path);
    bool saveRamFileIfSupported(const std::string& path) const;

    const std::string& getGameTitle() const;

protected:
    // 0000-3FFF: 16KB ROM Bank 00 (in cartridge, fixed at bank 00)
    // 4000-7FFF: 16KB ROM Bank 01..NN (in cartridge, switchable bank number)
    std::vector<uint8_t> m_rom;

    // A000-BFFF: 8KB External RAM (in cartridge, switchable bank, if any)
    std::vector<uint8_t> m_ram;

    // 0100-014F: Cartridge Header
    CartridgeHeader m_header;
};

class NoMBC : public Cartridge {
public:
    NoMBC(std::vector<uint8_t> rom, std::vector<uint8_t> ram, CartridgeHeader header);

    uint8_t readByte(uint16_t address) const override;
    void writeByte(uint16_t address, uint8_t value) override;
};

class MBC1 : public Cartridge {
public:
    MBC1(std::vector<uint8_t> rom, std::vector<uint8_t> ram, CartridgeHeader header);

    uint8_t readByte(uint16_t address) const override;
    void writeByte(uint16_t address, uint8_t value) override;

private:
    // 0000-1FFF: RAM Enable (write only; lower 4 bits)
    //  - 00: Disable RAM (default)
    //  - 0A: Enable RAM
    bool m_ramEnable = false;

    // 2000-3FFF: ROM Bank Number (write only)
    // Selects the lower 5 bits of the ROM Bank Number (in range 01-1F)
    uint8_t m_romBankNumber = 0x00;

    // 4000-5FFF: RAM Bank Number / Upper Bits of ROM Bank Number (write only)
    // Selects the 2-bit RAM Bank Number (in range 00-03) or the upper 2 bits
    // of the ROM Bank Number, depending on the ROM/RAM Mode Select.
    uint8_t m_ramBankNumber = 0x00;

    // 6000-7FFF: ROM/RAM Mode Select (write only)
    // Selects whether the above register should be used as the upper 2 bits
    // of the ROM Bank Number or as the RAM Bank Number.
    //  - 00 = ROM Banking Mode (up to 8KB RAM, 2MB ROM) (default)
    //  - 01 = RAM Banking Mode (up to 32KB RAM, 512KB ROM)
    bool m_romRamModeSelect = false;
};

class MBC3 : public Cartridge {
public:
    MBC3(std::vector<uint8_t> rom, std::vector<uint8_t> ram, CartridgeHeader header);

    uint8_t readByte(uint16_t address) const override;
    void writeByte(uint16_t address, uint8_t value) override;

private:
    // 0000-1FFF: RAM and Timer Enable (write only; lower 4 bits)
    //  - 00: Disable RAM and Timer (default)
    //  - 0A: Enable RAM and Timer
    bool m_ramAndTimerEnable = false;

    // 2000-3FFF: ROM Bank Number (write only)
    // Same as for MBC1, except that the whole 7 bits of the ROM Bank Number are
    // written directly to this address. As for the MBC1, writing a value of 00h,
    // will select Bank 01h instead.
    uint8_t m_romBankNumber = 0x00;

    // 4000-5FFF: RAM Bank Number or RTC Register Select (write only)
    // As in MBC1's RAM Banking mode, writing a value in range for 00h-03h maps
    // the corresponding external RAM Bank (if any) into memory at A000-BFFF. However,
    // when writing a value of 08h-0Ch, this will map the corresponding RTC register
    // into memory at A000-BFFF. That register could then be read/written by accessing
    // any address in that area, typically that is done by using address A000.
    uint8_t m_ramBankNumberOrRtcRegisterSelect = 0x00;

    // 6000-7FFF: Latch Clock Data (write only)
    // When writing 00h, and then 01h to this register, the current time becomes latched
    // into the RTC registers. The latched data will not change until it becomes latched
    // again, by repeating the write 00h->01h procedure. It is provided so that the latched
    // (frozen) time may be read while the clock continues to tick in the background.
    uint8_t m_latchClockData = 0x02;

    // The Clock Counter Registers (read/write)
    //  - 08h  RTC S   Seconds   0-59 (0-3Bh)
    uint8_t m_rtcSeconds = 0x00;
    //  - 09h  RTC M   Minutes   0-59 (0-3Bh)
    uint8_t m_rtcMinutes = 0x00;
    //  - 0Ah  RTC H   Hours     0-23 (0-17h)
    uint8_t m_rtcHours = 0x00;
    //  - 0Bh  RTC DL  Lower 8 bits of Day Counter (0-FFh)
    uint8_t m_rtcDaysLower = 0x00;
    //  - 0Ch  RTC DH  Upper 1 bit of Day Counter, Carry Bit, Halt Flag
    //    - Bit 0  Most significant bit of Day Counter (Bit 8)
    //    - Bit 6  Halt (0=Active, 1=Stop Timer)
    //    - Bit 7  Day Counter Carry Bit (1=Counter Overflow)
    uint8_t m_rtcDaysHigher = 0x00;

    // Time since the clock started/was reset
    std::chrono::time_point<std::chrono::system_clock> clockStartTime = std::chrono::system_clock::now();

    // Notes: The Day Counter
    // The total 9 bits of the Day Counter allow to count days in range from 0-511 (0-1FFh).
    // The Day Counter Carry Bit becomes set when this value overflows. In that case the Carry
    // Bit remains set until the program resets it.
};

class MBC5 : public Cartridge {
public:
    MBC5(std::vector<uint8_t> rom, std::vector<uint8_t> ram, CartridgeHeader header);

    uint8_t readByte(uint16_t address) const override;
    void writeByte(uint16_t address, uint8_t value) override;

private:
    // 0000-1FFF: RAM Enable (write only; lower 4 bits)
    //  - 00: Disable RAM (default)
    //  - 0A: Enable RAM
    bool m_ramEnable = false;

    // 2000-2FFF: Lower 8 bits of ROM Bank Number (write only)
    // The lower 8 bits of the ROM bank number are stored here. Writing 0 will
    // actually result in bank 0, unlike other MBCs.
    uint8_t m_romBankNumberLower = 0x00;

    // 3000-3FFF: Highest 1 bit of ROM Bank Number (write only)
    // The highest (8th) bit of the ROM Bank Number.
    uint8_t m_romBankNumberHigher = 0x00;

    // 4000-5FFF: RAM Bank Number (write only)
    // Selects the 5-bit RAM bank number (in range 00-0F)
    uint8_t m_ramBankNumber = 0x00;
};

std::unique_ptr<Cartridge> makeCartridge(std::vector<uint8_t> rom);
std::unique_ptr<Cartridge> loadRomFile(const std::string& path);

#endif //BIGBOY_CARTRIDGE_H
