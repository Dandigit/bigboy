#include <bigboy/GPU/GPU.h>

#include <iostream>

const std::array<Pixel, 160*144>& GPU::getCurrentFrame() const {
    return m_frameBuffer;
}

bool GPU::step(uint8_t cycles) {
    m_clock += cycles;

    switch (m_mode) {
        case GPUMode::HORIZONTAL_BLANK:
            if (m_clock >= 204) {
                ++m_currentLine;

                if (m_currentLine == 143) {
                    switchMode(GPUMode::VERTICAL_BLANK);
                    // Tell the caller we have finished rendering the frame
                    return true;
                } else {
                    switchMode(GPUMode::SCANLINE_OAM);
                }
            }
            break;
        case GPUMode::VERTICAL_BLANK:
            if (m_clock >= 456) {
                ++m_currentLine;

                if (m_currentLine == 153) {
                    m_currentLine = 0;
                    switchMode(GPUMode::SCANLINE_OAM);
                }
            }
        case GPUMode::SCANLINE_OAM:
            if (m_clock >= 80) {
                switchMode(GPUMode::SCANLINE_VRAM);
            }
            break;
        case GPUMode::SCANLINE_VRAM:
            if (m_clock >= 172) {
                renderScanline();
                switchMode(GPUMode::HORIZONTAL_BLANK);
            }
            break;
    }

    // We are not yet finished rendering the frame
    return false;
}

void GPU::renderScanline() {
    renderBackgroundScanline();
    m_frameBuffer = m_bgBuffer;
}

void GPU::renderBackgroundScanline() {
    // Index of the current row in the framebuffer
    size_t rowStart = m_currentY * 160 * 4;

    if (!bgEnable()) {
        // Clear the row, the background is disabled
        for (size_t x = 0; x < 160; ++x) {
            m_bgBuffer[rowStart + x] = Pixel::OFF;
        }
        return;
    }

    // Are we using tile map 1 (8000) or 0 (8800)?
    // Get the start index in VRAM.
    uint16_t tilemapIndex = (tileMap() ? 0x8000 : 0x8800) - 0x8000;

    // Are we using background tile set 1 (9C00) or 0 (9800)?
    // Get the start index in VRAM.
    uint16_t tilesetIndex = (bgTileset() ? 0x9C00 : 0x9800) - 0x8000;

    // Get the row of the tiles that we will retrieve from the tile map
    uint16_t row = ((m_scrollY + m_currentY) / 8) % 32;

    // Get the row offset within the tile (8 rows)
    uint16_t rowOffset = (m_scrollY + m_currentY) % 8;

    // For each pixel on this line
    for (uint8_t x = 0; x < 160; ++x) {
        // Get the column of the tile that we will retrieve from the tile map
        uint16_t column = ((m_scrollX + x) / 8) % 32;

        // Get the tile number within the tile set
        uint8_t tileNumber = m_vram[tilemapIndex + (row * 32) + column];

        uint16_t tileIndex;
        if (bgTileset() == 0) {
            // The tile number is signed
            tileIndex = 255 + static_cast<int8_t>(tileNumber);
        } else {
            // The tile number is unsigned
            tileIndex = tileNumber;
        }

        // Retrieve the tile we will use
        Tile tile = m_tileset[tileIndex];

        // Get the index of the pixel colour
        TileValue colourIndex = tile[rowOffset][x];

        // Get the colour that this refers to
        Pixel colour = getPaletteColour(static_cast<uint8_t>(colourIndex));

        m_bgBuffer[rowStart + x] = colour;
    }
}

void GPU::updateTileset(uint16_t address) {
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

void GPU::switchMode(GPUMode newMode) {
    m_clock = 0;
    m_mode = newMode;
}

void GPU::reset() {
    m_scrollY = 0x00;
    m_scrollX = 0x00;
    m_currentYCompare = 0x00;
    m_backgroundPalette = 0xFC;
    m_spritePalette0 = 0xFF;
    m_spritePalette1 = 0xFF;
    m_windowY = 0x00;
    m_windowX = 0x00;
}

std::vector<AddressSpace> GPU::addressSpaces() const {
    return {{0x8000, 0x9FFF}, {0xFE00, 0xFE9F}, {0xFF40, 0xFF4B}};
}

uint8_t GPU::readByte(uint16_t address) const {
    // Registers?
    switch (address) {
        case 0xFF40: return m_control;
        case 0xFF41: return m_status;
        case 0xFF42: return m_scrollY;
        case 0xFF43: return m_scrollX;
        case 0xFF44: return m_currentY;
        case 0xFF45: return m_currentYCompare;
        case 0xFF46: std::cerr << "Cannot read DMA!" << '\n';
        case 0xFF47: return m_backgroundPalette;
        case 0xFF48: return m_spritePalette0;
        case 0xFF49: return m_spritePalette1;
        case 0xFF4A: return m_windowY;
        case 0xFF4B: return m_windowX;
        default:     break;
    }

    if (address >= 0x8000 && address <= 0x9FFF) {
        return m_vram[0x8000 - address];
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        // TODO: Sprites
        std::cerr << "Sprites are not yet implemented!" << '\n';
        return 0xFF;
    }

    std::cerr << "Memory device GPU does not support reading the address " +
            address << '\n';
    return 0xFF;
}

void GPU::writeByte(uint16_t address, uint8_t value) {
    // Registers?
    switch (address) {
        case 0xFF40:
            m_control = value;
            return;
        case 0xFF41:
            m_status = value;
            return;
        case 0xFF42:
            m_scrollY = value;
            return;
        case 0xFF43:
            m_scrollX = value;
            return;
        case 0xFF44:
            std::cerr << "Cannot write to LY." << '\n';
            return;
        case 0xFF45:
            m_currentYCompare = value;
            return;
        case 0xFF46:
            std::cerr << "DMA is not yet implemented!" << '\n';
            return;
        case 0xFF47:
            m_backgroundPalette = value;
            return;
        case 0xFF48:
            m_spritePalette0 = value;
            return;
        case 0xFF49:
            m_spritePalette1 = value;
            return;
        case 0xFF4A:
            m_windowY = value;
            return;
        case 0xFF4B:
            m_windowX = value;
            return;
        default:
            break;
    }

    if (address >= 0x8000 && address <= 0x9FFF) {
        m_vram[0x8000 - address] = value;

        // Check if we updated the tileset
        if (address <= 0x97FF) {
            updateTileset(address);
        }
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        // TODO: Sprites
        std::cerr << "Sprites are not yet implemented!" << '\n';
    } else {
        std::cerr << "Memory device GPU does not support reading the address " +
                address << '\n';
    }
}
