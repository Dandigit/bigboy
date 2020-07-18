#ifndef BIGBOY_CARTRIDGE_H
#define BIGBOY_CARTRIDGE_H

#include <array>

#include <bigboy/Cartridge/CartridgeHeader.h>
#include <bigboy/MMU/MemoryDevice.h>

class Cartridge : public MemoryDevice {
public:
    Cartridge(std::vector<uint8_t> rom, std::vector<uint8_t> ram, CartridgeHeader header);
    virtual ~Cartridge() = default;

    std::vector<AddressSpace> addressSpaces() const override;

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
    ~MBC1() override;

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

std::unique_ptr<Cartridge> makeCartridge(std::vector<uint8_t> rom, std::vector<uint8_t> ram);
std::unique_ptr<Cartridge> readCartridgeFile(const std::string& filename);

#endif //BIGBOY_CARTRIDGE_H
