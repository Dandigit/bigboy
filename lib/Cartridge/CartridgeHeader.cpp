#include <bigboy/Cartridge/CartridgeHeader.h>

#include <vector>
#include <iostream>

CartridgeHeader makeCartridgeHeader(const std::vector<uint8_t>& rom) {
    auto cgbFlag = static_cast<CGBFlag>(rom[0x0143]);
    auto mbcType = static_cast<MBCType>(rom[0x0147]);
    auto romSize = static_cast<ROMSize>(rom[0x0148]);
    auto ramSize = static_cast<RAMSize>(rom[0x0149]);
    auto destinationCode = static_cast<DestinationCode>(rom[0x014A]);

    const size_t titleEndIndex = (cgbFlag == CGBFlag::CGB_GB || cgbFlag == CGBFlag::CGB)
            ? 0x0142
            : 0x0143;
    std::string title;

    for (size_t i = 0x0134; i <= titleEndIndex; ++i) {
        auto c = static_cast<unsigned char>(rom[i]);

        if (c == '\0') break;
        title.push_back(c);
    }

    return CartridgeHeader{std::move(title), mbcType, romSize, ramSize, destinationCode};
}

uint32_t ramSizeInBytes(const RAMSize ramSize) {
    switch (ramSize) {
        case RAMSize::NONE: return 0;
        case RAMSize::KB_2: return 2048;
        case RAMSize::KB_8: return 8096;
        case RAMSize::KB_32: return 32768;
    }
}

std::string serialise(const MBCType mbcType) {
    switch (mbcType) {
        case MBCType::ROM: return "ROM";
        case MBCType::MBC1: return "MBC1";
        case MBCType::MBC1_RAM: return "MBC1_RAM";
        case MBCType::MBC1_RAM_BATTERY: return "MBC1_RAM_BATTERY";
        case MBCType::MBC2: return "MBC2";
        case MBCType::MBC2_BATTERY: return "MBC2_BATTERY";
        case MBCType::ROM_RAM: return "ROM_RAM";
        case MBCType::ROM_RAM_BATTERY: return "ROM_RAM_BATTERY";
        case MBCType::MMM01: return "MMM01";
        case MBCType::MMM01_RAM: return "MMM01_RAM";
        case MBCType::MMM01_RAM_BATTERY: return "MMM01_RAM_BATTERY";
        case MBCType::MBC3_TIMER_BATTERY: return "MBC3_TIMER_BATTERY";
        case MBCType::MBC3_TIMER_RAM_BATTERY: return "MBC3_TIMER_RAM_BATTERY";
        case MBCType::MBC3: return "MBC3";
        case MBCType::MBC3_RAM: return "MBC3_RAM";
        case MBCType::MBC3_RAM_BATTERY: return "MBC3_RAM_BATTERY";
        case MBCType::MBC4: return "MBC4";
        case MBCType::MBC4_RAM: return "MBC4_RAM";
        case MBCType::MBC4_RAM_BATTERY: return "MBC4_RAM_BATTERY";
        case MBCType::MBC5: return "MBC5";
        case MBCType::MBC5_RAM: return "MBC5_RAM";
        case MBCType::MBC5_RAM_BATTERY: return "MBC5_RAM_BATTERY";
        case MBCType::MBC5_RUMBLE: return "MBC5_RUMBLE";
        case MBCType::MBC5_RUMBLE_RAM: return "MBC5_RUMBLE_RAM";
        case MBCType::MBC5_RUMBLE_RAM_BATTERY: return "MBC5_RUMBLE_RAM_BATTERY";
        case MBCType::POCKET_CAMERA: return "POCKET_CAMERA";
        case MBCType::BANDAI_TAMA5: return "BANDAI_TAMA5";
        case MBCType::HUC3: return "HUC3";
        case MBCType::HUC1_RAM_BATTERY: return "HUC1_RAM_BATTERY";
    }
}