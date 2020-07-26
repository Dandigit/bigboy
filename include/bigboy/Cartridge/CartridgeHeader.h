#ifndef BIGBOY_CARTRIDGEHEADER_H
#define BIGBOY_CARTRIDGEHEADER_H

#include <cstdint>
#include <string>
#include <vector>

// 0143: CGB Flag
// In older cartridges this byte has been part of the Title (see above).
// In CGB cartridges the upper bit is used to enable CGB functions.
// This is required, otherwise the CGB switches itself into Non-CGB-Mode.
//  - 80h: Game supports CGB functions, but works on old gameboys also.
//  - C0h: Game works on CGB only (physically the same as 80h).
// Values with Bit 7 set, and either Bit 2 or 3 set, will switch the gameboy
// into a special non-CGB-mode with uninitialized palettes. Purpose unknown,
// eventually this has been supposed to be used to colorize monochrome games
// that include fixed palette data at a special location in ROM.
enum class CGBFlag {
    GB = 0b01,     // Uses GB features only
    GB_CGB = 0b11, // Uses CGB features but works on GB
    CGB = 0b10,    // Uses CGB features and does not work on GB
};

// 0147: Cartridge Type
// Specifies which Memory Bank Controller (if any) is used in
// the cartridge, and if further external hardware exists
enum class MBCType : uint8_t {
    ROM                     = 0x00,
    MBC1                    = 0x01,
    MBC1_RAM                = 0x02,
    MBC1_RAM_BATTERY        = 0x03,
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
// Specifies if this version of the game is supposed to be sold in Japan,
// or anywhere else. Only two values are defined.
enum class DestinationCode {
    JAPAN     = 0x00,
    NOT_JAPAN = 0x01,
};

struct CartridgeHeader {
    std::string title;
    MBCType mbcType;
    ROMSize romSize;
    RAMSize ramSize;
    DestinationCode destinationCode;
};

CartridgeHeader makeCartridgeHeader(const std::vector<uint8_t>& rom);

CGBFlag toCGBFlag(uint8_t byte);
uint32_t ramSizeInBytes(RAMSize ramSize);

std::string serialise(MBCType mbcType);
std::string serialise(ROMSize romSize);
std::string serialise(RAMSize ramSize);
std::string serialise(DestinationCode destinationCode);

#endif //BIGBOY_CARTRIDGEHEADER_H
