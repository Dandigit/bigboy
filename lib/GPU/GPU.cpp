#include <bigboy/GPU/GPU.h>

#include <iostream>

const std::array<Pixel, 160*144>& GPU::getCurrentFrame() const {
    return m_frameBuffer;
}

GPU::Request GPU::update(uint8_t cycles) {
    m_clock += cycles;

    bool requestVblank = false;
    bool requestStat = false;

    switch (getMode()) {
        case GPUMode::HORIZONTAL_BLANK:
            if (m_clock >= 204) {
                m_clock -= 204;
                ++m_currentY;

                if (m_currentY == 144) {
                    // Request a VBLANK interrupt!
                    requestVblank = true;
                    requestStat = switchMode(GPUMode::VERTICAL_BLANK);
                } else {
                    requestStat = switchMode(GPUMode::SCANLINE_OAM);
                }
            }
            break;
        case GPUMode::VERTICAL_BLANK:
            if (m_clock >= 456) {
                m_clock -= 456;
                ++m_currentY;

                if (m_currentY == 154) {
                    m_currentY = 0;
                    requestStat = switchMode(GPUMode::SCANLINE_OAM);
                }
            }
            break;
        case GPUMode::SCANLINE_OAM:
            if (m_clock >= 80) {
                m_clock -= 80;
                requestStat = switchMode(GPUMode::SCANLINE_VRAM);
            }
            break;
        case GPUMode::SCANLINE_VRAM:
            if (m_clock >= 172) {
                m_clock -= 172;
                renderScanline();
                requestStat = switchMode(GPUMode::HORIZONTAL_BLANK);
            }
            break;
    }

    // LYC STAT interrupt?
    if (m_currentYCompare == m_currentY &&
            statInterruptEnabled(StatInterrupt::LYC)) {
        setCoincidenceFlag();
        requestStat = true;
    } else {
        clearCoincidenceFlag();
    }

    return Request{requestVblank, requestStat};
}

void GPU::reset() {
    m_control = 0x91;
    m_scrollY = 0x00;
    m_scrollX = 0x00;
    m_currentY = 145;
    m_currentYCompare = 0x00;
    m_bgPalette = 0xFC;
    m_spritePalette0 = 0xFF;
    m_spritePalette1 = 0xFF;
    m_windowY = 0x00;
    m_windowX = 0x00;
    switchMode(GPUMode::VERTICAL_BLANK);
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
        case 0xFF46: std::cerr << "Cannot read DMA!" << '\n'; return 0xFF;
        case 0xFF47: return m_bgPalette;
        case 0xFF48: return m_spritePalette0;
        case 0xFF49: return m_spritePalette1;
        case 0xFF4A: return m_windowY;
        case 0xFF4B: return m_windowX;
        default:     break;
    }

    if (address >= 0x8000 && address <= 0x9FFF) {
        return m_vram[address - 0x8000];
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        // TODO: Sprites
        std::cerr << "Sprites are not yet implemented!" << '\n';
        return 0xFF;
    }

    std::cerr << "Memory device GPU does not support reading the address " << address << '\n';
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
            m_currentY = 0;
            return;
        case 0xFF45:
            m_currentYCompare = value;
            return;
        case 0xFF46:
            std::cerr << "DMA is not yet implemented!" << '\n';
            return;
        case 0xFF47:
            m_bgPalette = value;
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

        /*// Check if we updated the tileset
        if (address <= 0x97FF) {
            updateTileset(address);
        }*/
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        // TODO: Sprites
        std::cerr << "Sprites are not yet implemented!" << '\n';
    } else {
        std::cerr << "Memory device GPU does not support reading the address " +
                     address << '\n';
    }
}

void GPU::renderScanline() {
    renderBackgroundScanline();
    m_frameBuffer = m_bgBuffer;
}

void GPU::renderBackgroundScanline() {
    if (!bgEnable()) {
        // If BG is disabled, render a white background and exit early
        for (int x = 0; x < 160; x++) {
            int index = (m_currentY * 160) + x;
            m_bgBuffer[index] = Pixel::OFF;
        }

        return;
    }

    // Where in VRAM is our background tileset?
    // The bgTileset flag indicates which tileset we are using; 1 (0x9C00) or 0 (0x9800).
    // We subtract 0x8000 so we can index directly into VRAM.
    uint16_t tilesetIndex = (bgTileset() ? 0x9C00 : 0x9800) - 0x8000;

    // Bit 4 - BG & Window Tile Data Select   (0=8800-97FF, 1=8000-8FFF)
    // Now, which tiles are we actually rendering? We find this in the tile map, a
    // 32*32 set of indexes into the currently selected tileset. There are two tile maps
    // in memory, the tileMap flag indicated which we are using; 1 (0x8000), or 0 (0x8800).
    // Because tile map 0 uses SIGNED indexes (-127 to 127), we start in the middle, at 0x9000.
    // Again, we subtract 0x8000 for direct VRAM access.
    uint16_t tilemapIndex = (tileMap() ? 0x8000 : 0x9000) - 0x8000;

    // Which row of tiles corresponds to the current scanline? Well, each tile is 8*8 pixels, so
    // we divide by 8, and the tile map is 32*32 tiles, so we mod to find the tile that this line
    // of pixels belongs to. Note that we needed to add the Y scroll offset (SCY) first.
    uint8_t tileY = ((m_currentY + m_scrollY) / 8) % 32;

    // Which pixel row of the tile row are we talking about? Tiles are 8*8, so we mod by 8
    // to find this.
    uint8_t tileYOffset = (m_currentY + m_scrollY) % 8;

    // Loop through each pixel on the line:
    for (uint8_t x = 0; x < 160; x++) {
        // Now, which column of tiles corresponds to our current X position? Again, we divide
        // by 8 (tiles have 8 columns) and mod by 32 (tile map has 32 columns).
        uint8_t tileX = ((m_scrollX + x) / 8) % 32;

        // We can now read into the tile map to find the index (into the selected tileset) of
        // the tile we need to render
        uint8_t tileIndex = m_vram[tilesetIndex + (tileY * 32) + tileX];

        // Depending on the currently selected tilemap, this index may be signed or unsigned.
        // Tilemap 1 uses unsigned indexes, whereas tilemap 0 uses signed indexes. Either way,
        // we need to find the absolute index of the tile within VRAM.
        uint16_t absoluteTileIndex;
        if (tileMap() == 1) {
            // Unsigned index
            absoluteTileIndex = tilemapIndex + tileIndex * 0x10;
        } else {
            // Signed index
            absoluteTileIndex = tilemapIndex + static_cast<int8_t>(tileIndex) * 0x10;
        }

        // Now we find the index of the pixel row we are talking about, using the offset we
        // determined earlier, Each row is 2 bytes long, so we must multiply our offset by 2
        uint16_t pixelRowIndex = absoluteTileIndex + tileYOffset * 2;

        // Read the two bytes!
        uint8_t rowLow = m_vram[pixelRowIndex];
        uint8_t rowHigh = m_vram[pixelRowIndex + 1];

        // Now we obtain the low and high bits of the pixel we are looking at. The low bit
        // is stored at the xth position in the low byte of the row, and the high bit is stored
        // at the xth position in the high byte. We must offset by the X scroll here as well when
        // determining the current X position.
        uint8_t pos = 7 - ((m_scrollX + x) % 8);
        uint8_t pixelLow = ((rowLow >> pos) & 1u) ? 0x01 : 0x00;
        uint8_t pixelHigh = ((rowHigh >> pos) & 1u) ? 0x02 : 0x00;

        // Get the colour
        Pixel colour = getPaletteColour(pixelLow + pixelHigh);

        // We find the index of this pixel within our background framebuffer:
        size_t index = (m_currentY * 160) + x;

        // And set it to the appropriate colour.
        m_bgBuffer[index] = colour;
    }
}

bool GPU::switchMode(GPUMode newMode) {
    const auto modeBits = static_cast<uint8_t>(newMode);

    // Set/clear bit 0 of STAT
    const bool bit0Set = (modeBits & 1u) != 0;
    m_status = (m_status & ~1u) | bit0Set;

    // Set/clear bit 1 of STAT
    const bool bit1Set = (modeBits & (1u << 1)) != 0;
    m_status = (m_status & ~(1u << 1)) | (bit1Set << 1);

    // Should we request a STAT interrupt?
    switch (newMode) {
        case GPUMode::HORIZONTAL_BLANK:
            return statInterruptEnabled(StatInterrupt::HBLANK);
        case GPUMode::VERTICAL_BLANK:
            return statInterruptEnabled(StatInterrupt::VBLANK);
        case GPUMode::SCANLINE_OAM:
            return statInterruptEnabled(StatInterrupt::OAM);
        default:
            return false;
    }
}

Pixel GPU::getPaletteColour(uint8_t index) const {
    return static_cast<Pixel>(
            (m_bgPalette >> static_cast<uint8_t>(index)) & 0b11u);
}

