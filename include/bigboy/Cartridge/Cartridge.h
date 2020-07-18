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

private:
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
    MBC1(std::vector<uint8_t> rom, std::vector<uint8_t> ram, CartridgeHeader header);
    ~MBC1() override;

    uint8_t readByte(uint16_t address) const override;
    void writeByte(uint16_t address, uint8_t value) override;
};

std::unique_ptr<Cartridge> makeCartridge(std::vector<uint8_t> rom, std::vector<uint8_t> ram);

#endif //BIGBOY_CARTRIDGE_H
