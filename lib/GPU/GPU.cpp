#include <bigboy/GPU/GPU.h>

#include <string>

void GPU::updateTileset(uint16_t address, uint8_t value) {
    // Tile data is two bytes long, hence it always starts on an even address.
    const uint16_t tileAddress = address & 0xFFFEu;

    const uint8_t tileLow = m_vram[tileAddress];
    const uint8_t tileHigh = m_vram[tileAddress + 1];

    const uint16_t column = tileAddress / 16;
    const uint16_t row = (tileAddress % 16) / 2;

    for (uint8_t pixel = 0; pixel < 8; ++pixel) {
        // The low bit of the 2-bit colour code is stored in the low byte of the tile
        const uint8_t valueLow = (tileLow >> (7u - pixel)) & 1u;
        // And the high bit in the high byte
        const uint8_t valueHigh = (tileHigh >> (7u - pixel)) & 1u;

        // The value is these two bits together
        const auto value = static_cast<TileValue>((valueHigh << 1u) | valueLow);

        m_tileset[column][row][pixel] = value;
    }
}

std::vector<AddressSpace> GPU::addressSpaces() const {
    return {{0x8000, 0x9FFF}, {0xFE00, 0xFE9F}};
}

uint8_t GPU::readByte(uint16_t address) const {
    if (address >= 0x8000 && address <= 0x9FFF) {
        return m_vram[0x8000 - address];
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        // TODO: Sprites
        throw std::runtime_error{"Sprites are not yet implemented!"};
    }

    throw std::runtime_error{"Memory device GPU does not support reading the address " +
            std::to_string(address)};
}

void GPU::writeByte(uint16_t address, uint8_t value) {
    if (address >= 0x8000 && address <= 0x9FFF) {
        m_vram[0x8000 - address] = value;

        // Check if we updated the tileset
        if (address <= 0x8FFF) {
            updateTileset(address, value);
        }
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        // TODO: Sprites
        throw std::runtime_error{"Sprites are not yet implemented!"};
    } else {
        throw std::runtime_error{"Memory device GPU does not support reading the address " +
                std::to_string(address)};
    }
}
