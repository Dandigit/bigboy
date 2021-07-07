#include <bigboy/Cartridge.h>

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

bool Cartridge::loadRamFileIfSupported(const std::string& path) {
    switch (m_header.mbcType) {
        case MBCType::ROM_RAM_BATTERY:
        case MBCType::MBC1_RAM_BATTERY:
        case MBCType::MBC3_RAM_BATTERY:
        case MBCType::MBC3_TIMER_BATTERY:
        case MBCType::MBC3_TIMER_RAM_BATTERY:
        case MBCType::MBC5_RAM_BATTERY: {
            std::ifstream saveFile{path, std::ios::in | std::ios::binary | std::ios::ate};
            if (!saveFile.is_open()) {
                std::cerr << "warning: could not read save file '" << path << "'\n";
                return false;
            }

            saveFile.seekg(0, std::ios::end);
            const size_t length = saveFile.tellg();
            saveFile.seekg(0, std::ios::beg);

            if (m_ram.size() != length) {
                std::cerr << "warning: corrupt save file '" << path << "' was not read\n";
                return false;
            }

            saveFile.read(reinterpret_cast<char*>(m_ram.data()), length);
            std::cout << "note: read save file: " << length << " bytes\n";

            break;
        }
        default:
            break;
    }

    return true;
}

bool Cartridge::saveRamFileIfSupported(const std::string &filename) const {
    switch (m_header.mbcType) {
        case MBCType::ROM_RAM_BATTERY:
        case MBCType::MBC1_RAM_BATTERY:
        case MBCType::MBC3_RAM_BATTERY:
        case MBCType::MBC3_TIMER_BATTERY:
        case MBCType::MBC3_TIMER_RAM_BATTERY:
        case MBCType::MBC5_RAM_BATTERY: {
            std::ofstream saveFile{filename, std::ios::out | std::ios::binary | std::ios::trunc};
            if (!saveFile.is_open()) {
                std::cerr << "warning: could not write save file '" << filename << "'\n";
                return false;
            }

            saveFile.write(reinterpret_cast<const char*>(m_ram.data()), m_ram.size());
            std::cout << "note: wrote save file '" << filename << "', " << m_ram.size() << " bytes\n";

            break;
        }
        default:
            break;
    }

    return true;
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

    std::cerr << "warning: memory device Cartridge (" << serialise(m_header.mbcType) <<
            ") does not support reading the address " << std::to_string(address) << '\n';
    return 0xFF;
}

void NoMBC::writeByte(const uint16_t address, const uint8_t value) {
    if (address >= 0xA000 && address <= 0xBFFF &&
            (m_header.mbcType == MBCType::ROM_RAM ||
            m_header.mbcType == MBCType::ROM_RAM_BATTERY)) {
        m_ram[address - 0xA000] = value;
    } else {
        std::cerr << "warning: memory device Cartridge (" << serialise(m_header.mbcType) <<
                  ") does not support writing to the address " << std::to_string(address) << '\n';
    }
}

MBC1::MBC1(std::vector<uint8_t> rom, std::vector<uint8_t> ram, CartridgeHeader header) :
        Cartridge{std::move(rom), std::move(ram), std::move(header)} {
}

uint8_t MBC1::readByte(const uint16_t address) const {
    if (address >= 0x0000 && address <= 0x3FFF) {
        return m_rom[address];
    }
    if (address >= 0x4000 && address <= 0x7FFF) {
        const uint8_t romBankNumber = m_romRamModeSelect
                                      ? ((m_ramBankNumber << 5u) | m_romBankNumber)
                                      : m_romBankNumber;
        return m_rom[address - 0x4000 + 0x4000 * romBankNumber];
    }
    if (address >= 0xA000 && address <= 0xBFFF &&
            m_ramEnable &&
            (m_header.mbcType == MBCType::MBC1_RAM ||
            m_header.mbcType == MBCType::MBC1_RAM_BATTERY)) {
        const uint8_t ramBankNumber = m_romRamModeSelect ? 0 : m_ramBankNumber;
        return m_ram[address - 0xA000 + 0x2000 * ramBankNumber];
    }

    std::cerr << "warning: memory device Cartridge (" << serialise(m_header.mbcType) <<
              ") does not support reading the address " << std::to_string(address) << '\n';
    return 0xFF;
}

void MBC1::writeByte(const uint16_t address, const uint8_t value) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        m_ramEnable = (value & 0x0A) == 0x0A;
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
        std::cerr << "warning: memory device Cartridge (" << serialise(m_header.mbcType) <<
                  ") does not support writing to the address " << std::to_string(address) << '\n';
    }
}

MBC3::MBC3(std::vector<uint8_t> rom, std::vector<uint8_t> ram, CartridgeHeader header) :
        Cartridge{std::move(rom), std::move(ram), std::move(header)} {
}

uint8_t MBC3::readByte(uint16_t address) const {
    if (address >= 0x0000 && address <= 0x3FFF) {
        return m_rom[address];
    }
    if (address >= 0x4000 && address <= 0x7FFF) {
        return m_rom[address - 0x4000 + 0x4000 * m_romBankNumber];
    }
    if (address >= 0xA000 && address <= 0xBFFF &&
        m_ramAndTimerEnable) {
        // Are we addressing RAM?
        if (m_ramBankNumberOrRtcRegisterSelect >= 0x00 && m_ramBankNumberOrRtcRegisterSelect <= 0x03) {
            return m_ram[address - 0xA000 + 0x2000 * m_ramBankNumberOrRtcRegisterSelect];
        }
        // Are we addressing the RTC?
        switch (m_ramBankNumberOrRtcRegisterSelect) {
            case 0x08: return m_rtcSeconds;
            case 0x09: return m_rtcMinutes;
            case 0x0A: return m_rtcHours;
            case 0x0B: return m_rtcDaysLower;
            case 0x0C: return m_rtcDaysHigher;
            default:   break;
        }
    }

    std::cerr << "Memory device Cartridge (" << serialise(m_header.mbcType) <<
              ") does not support reading the address " << std::to_string(address) << '\n';
    return 0xFF;
}

void MBC3::writeByte(uint16_t address, uint8_t value) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        m_ramAndTimerEnable = (value & 0x0A) == 0x0A;
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        m_romBankNumber = (value == 0x00)
                          ? 0x01
                          : (value & 0b1111111u);
    } else if (address >= 0x4000 && address <= 0x5FFF) {
        m_ramBankNumberOrRtcRegisterSelect = value;
    } else if (address >= 0x6000 && address <= 0x7FFF) {
        if (m_latchClockData == 0x00 && value == 0x01) {
            m_latchClockData = 0x02;

            std::time_t sinceClockStartTime =
                    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) -
                    std::chrono::system_clock::to_time_t(clockStartTime);

            std::tm* latchedTm = new std::tm{
                m_rtcSeconds,
                m_rtcMinutes,
                m_rtcHours,
                0,0,0,0,
                (m_rtcDaysHigher << 8u) | m_rtcDaysLower,
            };
            std::time_t latchedTime = std::mktime(latchedTm) + sinceClockStartTime;
            delete latchedTm;
            latchedTm = std::localtime(&latchedTime);

            if (!latchedTm) {
                std::cerr << "warning: could not latch the clock\n";
                return;
            }

            m_rtcSeconds = latchedTm->tm_sec;
            m_rtcMinutes = latchedTm->tm_min;
            m_rtcHours = latchedTm->tm_hour;
            m_rtcDaysLower = (latchedTm->tm_yday & 0b11111111u);
            m_rtcDaysHigher = ((latchedTm->tm_yday >> 8u) & 1u);
        } else {
            m_latchClockData = value;
        }
    } else if (address >= 0xA000 && address <= 0xBFFF &&
               m_ramAndTimerEnable) {
        // Are we addressing RAM?
        if (m_ramBankNumberOrRtcRegisterSelect >= 0x00 && m_ramBankNumberOrRtcRegisterSelect <= 0x03 &&
                (m_header.mbcType == MBCType::MBC3_RAM ||
                 m_header.mbcType == MBCType::MBC3_RAM_BATTERY ||
                 m_header.mbcType == MBCType::MBC3_TIMER_RAM_BATTERY)) {
            m_ram[address - 0xA000 + 0x2000 * m_ramBankNumberOrRtcRegisterSelect] = value;
        }
        // Are we addressing the RTC?
        else if (m_ramBankNumberOrRtcRegisterSelect >= 0x08 && m_ramBankNumberOrRtcRegisterSelect <= 0x0C &&
                (m_header.mbcType == MBCType::MBC3_TIMER_BATTERY ||
                 m_header.mbcType == MBCType::MBC3_TIMER_RAM_BATTERY)) {
            switch (m_ramBankNumberOrRtcRegisterSelect) {
                case 0x08:
                    m_rtcSeconds = value;
                    break;
                case 0x09:
                    m_rtcMinutes = value;
                    break;
                case 0x0A:
                    m_rtcHours = value;
                    break;
                case 0x0B:
                    m_rtcDaysLower = value;
                    break;
                case 0x0C:
                    m_rtcDaysHigher = value;
                    break;
            }

            // Reset the clock
            clockStartTime = std::chrono::system_clock::now();
        } else {
            std::cerr << "warning: memory device Cartridge (" << serialise(m_header.mbcType) <<
                      ") does not support writing to the address " << std::to_string(address) << '\n';
        }
    } else {
        std::cerr << "warning: memory device Cartridge (" << serialise(m_header.mbcType) <<
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
        return m_rom[address - 0x4000 + 0x4000 * romBankNumber];
    }
    if (address >= 0xA000 && address <= 0xBFFF &&
        m_ramEnable &&
        (m_header.mbcType == MBCType::MBC5_RAM ||
         m_header.mbcType == MBCType::MBC5_RAM_BATTERY)) {
        return m_ram[address - 0xA000 + 0x2000 * m_ramBankNumber];
    }

    std::cerr << "warning: memory device Cartridge (" << serialise(m_header.mbcType) <<
              ") does not support reading the address " << std::to_string(address) << '\n';
    return 0xFF;
}

void MBC5::writeByte(const uint16_t address, const uint8_t value) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        m_ramEnable = (value & 0x0A) == 0x0A;
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
        std::cerr << "warning: memory device Cartridge (" << serialise(m_header.mbcType) <<
                  ") does not support writing to the address " << std::to_string(address) << '\n';
    }
}

std::unique_ptr<Cartridge> makeCartridge(std::vector<uint8_t> rom) {
    CartridgeHeader header = makeCartridgeHeader(rom);

    std::vector<uint8_t> ram;
    ram.resize(ramSizeInBytes(header.ramSize));

    switch (header.mbcType) {
        case MBCType::ROM:
        case MBCType::ROM_RAM:
        case MBCType::ROM_RAM_BATTERY:
            return std::make_unique<NoMBC>(std::move(rom), std::move(ram), std::move(header));
        case MBCType::MBC1:
        case MBCType::MBC1_RAM:
        case MBCType::MBC1_RAM_BATTERY:
            return std::make_unique<MBC1>(std::move(rom), std::move(ram), std::move(header));
        case MBCType::MBC3:
        case MBCType::MBC3_RAM:
        case MBCType::MBC3_RAM_BATTERY:
        case MBCType::MBC3_TIMER_BATTERY:
        case MBCType::MBC3_TIMER_RAM_BATTERY:
            return std::make_unique<MBC3>(std::move(rom), std::move(ram), std::move(header));
        case MBCType::MBC5:
        case MBCType::MBC5_RAM:
        case MBCType::MBC5_RAM_BATTERY:
            return std::make_unique<MBC5>(std::move(rom), std::move(ram), std::move(header));
        default:
            std::cerr << "fatal: unimplemented MBC type: " << serialise(header.mbcType) << '\n';
            std::exit(-1);
    }
}

std::unique_ptr<Cartridge> loadRomFile(const std::string& path) {
    std::ifstream file{path};
    if (!file.is_open()) {
        std::cerr << "warning: ROM file '" << path << "' could not be opened.";
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> rom{};
    rom.resize(length);
    file.read(reinterpret_cast<char*>(rom.data()), length);
    file.close();

    return makeCartridge(std::move(rom));
}