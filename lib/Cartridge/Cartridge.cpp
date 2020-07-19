#include <bigboy/Cartridge/Cartridge.h>

#include <algorithm>
#include <fstream>
#include <iostream>

Cartridge::Cartridge(std::vector<uint8_t> rom, std::vector<uint8_t> ram, CartridgeHeader header) :
        m_rom{std::move(rom)},
        m_ram{std::move(ram)},
        m_header{std::move(header)} {
}

std::vector<AddressSpace> Cartridge::addressSpaces() const {
    return {{0x0000, 0x7FFF}, {0xA000, 0xBFFF}};
}

const std::string& Cartridge::getGameTitle() const {
    return m_header.title;
}

NoMBC::NoMBC(std::vector<uint8_t> rom, std::vector<uint8_t> ram, CartridgeHeader header) :
        Cartridge{std::move(rom), std::move(ram), std::move(header)} {
}

uint8_t NoMBC::readByte(const uint16_t address) const {
    if (address >= 0x0000 && address <= 0x7FFF) {
        return m_rom[address];
    }
    if (address >= 0xA000 && address <= 0xBFFF) {
        switch (m_header.mbcType) {
            case MBCType::ROM_RAM:
            case MBCType::ROM_RAM_BATTERY:
                return m_ram[address - 0xA000];
            default:
                break;
        }
    }

    std::cerr << "Memory device Cartridge (" << serialise(m_header.mbcType) <<
            ") does not support reading the address " << std::to_string(address) << '\n';
    return 0xFF;
}

void NoMBC::writeByte(const uint16_t address, const uint8_t value) {
    if (address >= 0xA000 && address <= 0xBFFF &&
            (m_header.mbcType == MBCType::ROM_RAM ||
            m_header.mbcType == MBCType::ROM_RAM_BATTERY)) {
        m_ram[address - 0xA000] = value;
    } else {
        std::cerr << "Memory device Cartridge (" << serialise(m_header.mbcType) <<
                  ") does not support writing to the address " << std::to_string(address) << '\n';
    }
}

MBC1::MBC1(std::vector<uint8_t> rom, std::vector<uint8_t> ram, CartridgeHeader header) :
        Cartridge{std::move(rom), std::move(ram), std::move(header)} {
}

MBC1::~MBC1() {
    if (m_header.mbcType == MBCType::MBC1_RAM_BATTERY) {
        // TODO: save RAM
    }
}

uint8_t MBC1::readByte(const uint16_t address) const {
    if (address >= 0x0000 && address <= 0x3FFF) {
        return m_rom[address];
    }
    if (address >= 0x4000 && address <= 0x7FFF) {
        const uint8_t romBankNumber = m_romRamModeSelect
                                      ? ((m_ramBankNumber << 5u) | m_romBankNumber)
                                      : m_romBankNumber;
        return m_rom[address + 0x4000 * romBankNumber];
    }
    if (address >= 0xA000 && address <= 0xBFFF &&
            m_ramEnable &&
            (m_header.mbcType == MBCType::MBC1_RAM ||
            m_header.mbcType == MBCType::MBC1_RAM_BATTERY)) {
        const uint8_t ramBankNumber = m_romRamModeSelect ? 0 : m_ramBankNumber;
        return m_ram[address - 0xA000 + 0x2000 * ramBankNumber];
    }

    std::cerr << "Memory device Cartridge (" << serialise(m_header.mbcType) <<
              ") does not support reading the address " << std::to_string(address) << '\n';
    return 0xFF;
}

void MBC1::writeByte(const uint16_t address, const uint8_t value) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        m_ramEnable = (value & 0b1111u) == 0x0A;
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        m_romBankNumber = (value == 0x00)
                ? 0x01
                : (value & 0b11111u);
    } else if (address >= 0x4000 && address <= 0x5FFF) {
        m_ramBankNumber = (value & 0b11u);
    } else if (address >= 0x6000 && address <= 0x7FFF) {
        m_romRamModeSelect = ((value & 0b1u) == 0x01);
    } else if (address >= 0xA000 && address <= 0xBFFF &&
            m_ramEnable &&
            (m_header.mbcType == MBCType::MBC1_RAM ||
            m_header.mbcType == MBCType::MBC1_RAM_BATTERY)) {
        const uint8_t ramBankNumber = m_romRamModeSelect ? 0 : m_ramBankNumber;
        m_ram[address - 0xA000 + 0x2000 * ramBankNumber] = value;
    } else {
        std::cerr << "Memory device Cartridge (" << serialise(m_header.mbcType) <<
                  ") does not support writing to the address " << std::to_string(address) << '\n';
    }
}

MBC5::MBC5(std::vector<uint8_t> rom, std::vector<uint8_t> ram, CartridgeHeader header) :
        Cartridge{std::move(rom), std::move(ram), std::move(header)} {
}

uint8_t MBC5::readByte(const uint16_t address) const {
    if (address >= 0x0000 && address <= 0x3FFF) {
        return m_rom[address];
    }
    if (address >= 0x4000 && address <= 0x7FFF) {
        const uint16_t romBankNumber =
                (static_cast<uint16_t>(m_romBankNumberHigher) << 8u) |
                static_cast<uint16_t>(m_romBankNumberLower);
        return m_rom[address + 0x4000 * romBankNumber];
    }
    if (address >= 0xA000 && address <= 0xBFFF &&
        m_ramEnable &&
        (m_header.mbcType == MBCType::MBC1_RAM ||
         m_header.mbcType == MBCType::MBC1_RAM_BATTERY)) {
        return m_ram[address - 0xA000 + 0x2000 * m_ramBankNumber];
    }

    std::cerr << "Memory device Cartridge (" << serialise(m_header.mbcType) <<
              ") does not support reading the address " << std::to_string(address) << '\n';
    return 0xFF;
}

void MBC5::writeByte(const uint16_t address, const uint8_t value) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        m_ramEnable = (value & 0b1111u) == 0x0A;
    } else if (address >= 0x2000 && address <= 0x2FFF) {
        m_romBankNumberLower = value;
    } else if (address >= 0x3000 && address <= 0x3FFF) {
        m_romBankNumberHigher = (value & 0b1u);
    } else if (address >= 0x4000 && address <= 0x5FFF) {
        m_ramBankNumber = (value & 0b11111u);
    } else if (address >= 0xA000 && address <= 0xBFFF &&
               m_ramEnable &&
               (m_header.mbcType == MBCType::MBC1_RAM ||
                m_header.mbcType == MBCType::MBC1_RAM_BATTERY)) {
        m_ram[address - 0xA000 + 0x2000 * m_ramBankNumber] = value;
    } else {
        std::cerr << "Memory device Cartridge (" << serialise(m_header.mbcType) <<
                  ") does not support writing to the address " << std::to_string(address) << '\n';
    }
}

std::unique_ptr<Cartridge> makeCartridge(std::vector<uint8_t> rom, std::vector<uint8_t> ram) {
    CartridgeHeader header = makeCartridgeHeader(rom);
    switch (header.mbcType) {
        case MBCType::ROM:
        case MBCType::ROM_RAM:
        case MBCType::ROM_RAM_BATTERY:
            return std::make_unique<NoMBC>(std::move(rom), std::move(ram), std::move(header));
        case MBCType::MBC1:
        case MBCType::MBC1_RAM:
        case MBCType::MBC1_RAM_BATTERY:
            return std::make_unique<MBC1>(std::move(rom), std::move(ram), std::move(header));
        default:
            std::cerr << "Unimplemented MBC type: " << serialise(header.mbcType) << '\n';
            abort();
    }
}

std::unique_ptr<Cartridge> readCartridgeFile(const std::string& filename) {
    std::ifstream file{filename};
    if (!file.is_open()) {
        std::cerr << "File '" << filename << "' could not be opened.";
        abort();
    }

    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> rom{};
    rom.resize(length);
    file.read(reinterpret_cast<char*>(rom.data()), length);
    file.close();

    return makeCartridge(std::move(rom), std::vector<uint8_t>{});
}