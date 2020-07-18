#include <bigboy/Cartridge/Cartridge.h>

#include <algorithm>
#include <fstream>
#include <iostream>

Cartridge::Cartridge(const std::vector<uint8_t>& bytes) {
    if (bytes.size() < m_bank0.size()) {
        throw CartridgeLoadError{"Tried to load illegally small ROM."};
    }

    for (int i = 0; i < m_bank0.size(); i++) {
        m_bank0[i] = bytes[i];
    }

    m_bank1Onwards.resize(getROMSizeInBytes());
    m_extRAM.resize(getRAMSizeInBytes());

    std::copy(bytes.begin() + m_bank0.size(), bytes.end(),
            m_bank1Onwards.begin());

    switch (getMBCType()) {
        case MBCType::ROM_RAM_BATTERY:
        case MBCType::ROM_RAM:
        case MBCType::ROM:
            if (m_bank1Onwards.size() != 0x4000) {
                throw CartridgeLoadError{"Tried to load MBC0 ROM that was not 32KB in size."};
            }
            break;
        case MBCType::MBC1_RAM_BATTERY:
        case MBCType::MBC1_RAM:
        case MBCType::MBC1:
            if (m_bank1Onwards.size() != getROMSizeInBytes()) {
                throw CartridgeLoadError{"Tried to load MBC1 ROM of incorrect size."};
            }
            break;
        default:
            throw CartridgeLoadError{"Unimplemented MBC type: " +
                    std::to_string(static_cast<int>(getMBCType()))};
    }

    if (getMBCType() == MBCType::ROM_RAM_BATTERY ||
            getMBCType() == MBCType::MBC1_RAM_BATTERY) {
        const std::string saveFileName = getGameTitle() + ".gbsave";
        std::ifstream saveFile{saveFileName};
        if (saveFile.is_open()) {
            saveFile.seekg(0, std::ios::end);
            const size_t length = saveFile.tellg();
            saveFile.seekg(0, std::ios::beg);

            if (length != getRAMSizeInBytes()) {
                throw CartridgeLoadError{"Save file '" + saveFileName + "' is not the right size."};
            }

            m_extRAM.reserve(getRAMSizeInBytes());
            saveFile.read(reinterpret_cast<char*>(m_extRAM.data()), getRAMSizeInBytes());
        }
    }
}

Cartridge::~Cartridge() {
    switch (getMBCType()) {
        case MBCType::ROM_RAM_BATTERY: {
            const std::string saveFileName = getGameTitle() + ".gbsave";
            std::ofstream saveFile{saveFileName};
            if (!saveFile.is_open()) {
                throw CartridgeLoadError{"Could not write save file '" + saveFileName + "'."};
            }

            saveFile.write(reinterpret_cast<char*>(m_extRAM.data()), 0x2000);
            break;
        }
        default:
            break;
    }
}

Cartridge Cartridge::fromFile(std::string fileName) {
    std::ifstream file{fileName};
    if (!file.is_open()) {
        throw CartridgeLoadError{"[fatal]: File '" + fileName + "' could not be opened."};
    }

    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> bytes{};
    bytes.resize(length);
    file.read(reinterpret_cast<char*>(bytes.data()), length);
    file.close();

    return Cartridge{bytes};
}

std::vector<AddressSpace> Cartridge::addressSpaces() const {
    return {{0x0000, 0x7FFF}, {0xA000, 0xBFFF}};
}

uint8_t Cartridge::readByte(uint16_t address) const {
    if (address >= 0x0000 && address <= 0x3FFF) {
        return m_bank0[address];
    }
    if (address >= 0x4000 && address <= 0x7FFF) {
        switch (getMBCType()) {
            case MBCType::ROM:
            case MBCType::ROM_RAM:
            case MBCType::ROM_RAM_BATTERY:
                return m_bank1Onwards[address - 0x4000];
            case MBCType::MBC1:
            case MBCType::MBC1_RAM:
            case MBCType::MBC1_RAM_BATTERY:
                return m_bank1Onwards[address - 0x4000 + ((getROMBankNumber() - 1) * 0x4000)];
            default:
                break;
        }
    }
    if (address >= 0xA000 && address <= 0xBFFF) {
        switch (getMBCType()) {
            case MBCType::ROM_RAM:
            case MBCType::ROM_RAM_BATTERY:
                return m_extRAM[address - 0xA000];
            case MBCType::MBC1_RAM:
            case MBCType::MBC1_RAM_BATTERY:
                return m_extRAM[address - 0xA000 + ((m_ramBankSelect - 1) * 0xA000)];
            default:
                break;
        }
    }

    std::cerr << "Memory device Cartridge does not support reading the address: " << address << '\n';
    return 0xFF;
}

void Cartridge::writeByte(uint16_t address, uint8_t value) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        // Write the lowest 4 bits
        m_ramEnable &= ~0b1111u;
        m_ramEnable |= (value & 0b1111u);
    }

    if (address >= 0xA000 && address <= 0xBFFF) {
        switch (getMBCType()) {
            case MBCType::ROM:
            case MBCType::ROM_RAM:
            case MBCType::ROM_RAM_BATTERY:
                m_extRAM[address - 0xA000] = value;
                return;
            case MBCType::MBC1:
            case MBCType::MBC1_RAM:
            case MBCType::MBC1_RAM_BATTERY:
                m_extRAM[address - 0xA000 + ((m_ramBankSelect - 1) * 0xA000)] = value;
                return;
            default:
                break;
        }
    }

    std::cerr << "Memory device Cartridge does not support writing to the address: " << address << '\n';
}

std::string Cartridge::getGameTitle() const {
    std::string name{};
    for (int i = 0x0134; i < 0x0143; i++) {
        char c = m_bank0[i];
        if (c == '\0') break;

        name.push_back(c);
    }
    return name;
}

MBCType Cartridge::getMBCType() const {
    return static_cast<MBCType>(m_bank0[0x0147]);
}

ROMSize Cartridge::getROMSize() const {
    return static_cast<ROMSize>(m_bank0[0x0148]);
}

uint32_t Cartridge::getROMSizeInBytes() const {
    switch (getROMSize()) {
        case ROMSize::KB_32: return 16384;
        case ROMSize::KB_64: return 49152;
        case ROMSize::KB_128: return 114688;
        case ROMSize::KB_256: return 245760;
        case ROMSize::KB_512: return 5007904;
        case ROMSize::MB_1: return 1007616;
        case ROMSize::MB_2: return 2031616;
        case ROMSize::MB_4: return 4079616;
        case ROMSize::MB_1_1: return 1110016;
        case ROMSize::MB_1_2: return 1212416;
        case ROMSize::MB_1_5: return 1519616;
    }
}

RAMSize Cartridge::getRAMSize() const {
    return static_cast<RAMSize>(m_bank0[0x0149]);
}

uint32_t Cartridge::getRAMSizeInBytes() const {
    switch (getRAMSize()) {
        case RAMSize::NONE: return 0;
        case RAMSize::KB_2: return 2048;
        case RAMSize::KB_8: return 8192;
        case RAMSize::KB_32: return 32768;
    }
}

DestinationCode Cartridge::getDestinationCode() const {
    return static_cast<DestinationCode>(m_bank0[0x014A]);
}

bool Cartridge::isRAMEnabled() const {
    return m_ramEnable == 0x0A;
}

uint8_t Cartridge::getROMBankNumber() const {
    if (m_bankingModeSelect == 0) {
        return m_romBankSelect;
    }

    return (m_ramBankSelect << 5u) | m_romBankSelect;
}