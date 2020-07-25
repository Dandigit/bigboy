#include <bigboy/GPU/GPU.h>
#include <bigboy/MMU/MMU.h>

#include <iostream>

const std::array<Colour, 160 * 144>& GPU::getCurrentFrame() const {
    return m_frameBuffer;
}

GPU::Request GPU::update(uint8_t cycles) {
    if (!displayEnable()) {
        return Request{false, false};
    }

    if (m_dmaCountdown > 0) {
        m_dmaCountdown -= cycles;
    }

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
    m_dmaCountdown = 0;
    m_clock = 456;
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
    return {{0x8000, 0x9FFF},
            {0xFE00, 0xFE9F},
            {0xFF40, 0xFF4B}};
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
            return 0xFF;
        case 0xFF47: return m_bgPalette;
        case 0xFF48: return m_spritePalette0;
        case 0xFF49: return m_spritePalette1;
        case 0xFF4A: return m_windowY;
        case 0xFF4B: return m_windowX;
        default: break;
    }

    if (address >= 0x8000 && address <= 0x9FFF) {
        if (getMode() == GPUMode::SCANLINE_VRAM) {
            std::cerr << "Tried to read VRAM while GPU was in mode 3 (SCANLINE_VRAM)\n";
            return 0xFF; // Bogus value.
        }

        return m_vram[address - 0x8000];
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        return m_oam[address - 0xFE00];
    }

    std::cerr << "Memory device GPU does not support reading the address " << address << '\n';
    return 0xFF;
}

void GPU::writeByte(uint16_t address, uint8_t value) {
    // Registers?
    switch (address) {
        case 0xFF40: {
            const bool wasEnabled = displayEnable();
            m_control = value;
            if (wasEnabled && !displayEnable()) {
                // Display has been turned off. We need to clear the screen.
                m_frameBuffer.fill(COLOUR_0);
                m_currentY = 153;
                m_clock = 456;
                switchMode(GPUMode::VERTICAL_BLANK);
            }
            return;
        }
        case 0xFF41:
            // Only bits 3-7 are writable
            m_status = (value & 0xF8) | (m_status & 0x07);
            return;
        case 0xFF42:m_scrollY = value;
            return;
        case 0xFF43:m_scrollX = value;
            return;
        case 0xFF44:m_currentY = 0;
            return;
        case 0xFF45:m_currentYCompare = value;
            return;
        case 0xFF46:launchDMATransfer(value);
            return;
        case 0xFF47:m_bgPalette = value;
            return;
        case 0xFF48:m_spritePalette0 = value;
            return;
        case 0xFF49:m_spritePalette1 = value;
            return;
        case 0xFF4A:m_windowY = value;
            return;
        case 0xFF4B:m_windowX = value;
            return;
        default:break;
    }

    if (address >= 0x8000 && address <= 0x9FFF) {
        if (getMode() == GPUMode::SCANLINE_VRAM) {
            std::cerr << "Tried to write to VRAM while GPU was in mode 3 (SCANLINE_VRAM)\n";
            return; // Do nothing.
        }

        m_vram[address - 0x8000] = value;
    } else if (address >= 0xFE00 && address <= 0xFE9F) {
        m_oam[address - 0xFE00] = value;
    } else {
        std::cerr << "Memory device GPU does not support reading the address " <<
                  address << '\n';
    }
}

void GPU::launchDMATransfer(const uint8_t location) {
    const uint16_t start = location << 8u;

    for (uint8_t i = 0; i < 160; ++i) {
        m_oam[i] = m_mmu.readByte(start + i);
    }

    m_dmaCountdown = 752;
}

void GPU::renderScanline() {
    renderBackgroundScanline();
    if (windowEnable()) renderWindowScanline();
    if (spriteEnable()) renderSpriteScanline();
}

void GPU::renderBackgroundScanline() {
    if (!bgEnable()) {
        // If BG is disabled, render a white background and exit early
        for (int x = 0; x < 160; x++) {
            int index = (m_currentY * 160) + x;
            m_frameBuffer[index] = COLOUR_100;
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
        uint8_t tileIndex = m_vram[static_cast<uint16_t>(tilesetIndex + (tileY * 32) + tileX)];

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
        uint8_t rowHigh = m_vram[static_cast<uint16_t>(pixelRowIndex + 1)];

        // Now we obtain the low and high bits of the pixel we are looking at. The low bit
        // is stored at the xth position in the low byte of the row, and the high bit is stored
        // at the xth position in the high byte. We must offset by the X scroll here as well when
        // determining the current X position.
        uint8_t pos = 7 - ((m_scrollX + x) % 8);
        uint8_t pixelLow = (rowLow >> pos) & 1u;
        uint8_t pixelHigh = (rowHigh >> pos) & 1u;
        uint8_t pixel = (pixelHigh << 1u) | pixelLow;

        // And set it to the appropriate colour.
        m_frameBuffer[m_currentY * 160 + x] = getPaletteColour(m_bgPalette, pixel);
    }
}

void GPU::renderWindowScanline() {
    // Get the relative window Y position. If it is less than 0, we are off the screen.
    const int windowY = m_currentY - m_windowY;
    if (windowY < 0) return; // Draw nothing.

    // Where in VRAM is our window tileset?
    // The windowTileset flag indicates which tileset we are using; 1 (0x9C00) or 0 (0x9800).
    // We subtract 0x8000 so we can index directly into VRAM.
    uint16_t tilesetIndex = (tileMap() ? 0x8000 : 0x9000) - 0x8000;

    // Which of these tiles are we actually rendering?
    // We find this in the tile map, a 32*32 set of indexes into the currently selected tileset.
    // There are two tile maps in memory, the tileMap flag indicated which we are using; 1 (0x8000),
    // or 0 (0x8800). Because tile map 0 uses SIGNED indexes (-127 to 127), we start in the middle,
    // at 0x9000. Again, we subtract 0x8000 for direct VRAM access.
    uint16_t tilemapIndex = (windowTileset() ? 0x9C00 : 0x9800) - 0x8000;

    // Tiles are 8 pixels tall, so we figure out which tile we need by dividing our current Y pos by 8.
    uint8_t tileYIndex = windowY / 8;
    // The offset (specific pixel row) into said tile is the remainder.
    uint8_t tileYOffset = windowY % 8;

    // Get the relative window X position and loop through each pixel on the current line.
    const int windowX = m_windowX - 7;
    for (int x = 0; x < 160; ++x) {
        // If the window is to the right of here, we don't draw anything
        if (x < windowX) continue;

        // Tiles are 8 pixels wide, so we figure out which tile we need by dividing our current X pos by 8.
        uint8_t tileXIndex = (x - windowX) / 8;

        // Now, we can get the index of the tile from the tile map in VRAM.
        uint8_t tileIndex = m_vram[static_cast<uint16_t>(tilemapIndex + (tileYIndex * 32) + tileXIndex)];

        // Depending on the currently selected tilemap, this index may be signed or unsigned.
        // Tilemap 1 uses unsigned indexes, whereas tilemap 0 uses signed indexes. Either way,
        // we need to find the absolute index of the tile within VRAM.
        uint16_t absoluteTileIndex = (tileMap() == 1)
                                     ? static_cast<uint16_t>(tilesetIndex + tileIndex * 0x10)
                                     : static_cast<uint16_t>(tilesetIndex + static_cast<int8_t>(tileIndex) * 0x10);

        // We find the index of the row we are talking about by adding our Y offset, times two, as
        // rows are two bytes long.
        uint16_t rowIndex = absoluteTileIndex + tileYOffset * 2;

        // Read the row data
        uint8_t rowLow = m_vram[rowIndex];
        uint8_t rowHigh = m_vram[static_cast<uint16_t>(rowIndex + 1)];

        // From the row, read the pixel that we are currently looking at
        uint8_t pos = 7 - x % 8;
        uint8_t pixelLow = (rowLow >> pos) & 1u;
        uint8_t pixelHigh = (rowHigh >> pos) & 1u;
        uint8_t pixel = (pixelHigh << 1u) | pixelLow;

        m_frameBuffer[m_currentY * 160 + x] = getPaletteColour(m_bgPalette, pixel);
    }
}

void GPU::renderSpriteScanline() {
    // Walk the Sprite Attribute Table backwards
    for (int i = 156; i >= 0; i -= 4) {
        const uint8_t spriteYMinus16 = m_oam[i];
        const uint8_t spriteY = spriteYMinus16 - 16;
        const uint8_t spriteHeight = spriteSize() ? 16 : 8;

        const uint8_t flags = m_oam[i + 3];

        // Ensure that the sprite is on the current scanline
        if (spriteY > m_currentY || (spriteY + spriteHeight) <= m_currentY) continue;

        const uint8_t spriteXMinus8 = m_oam[i + 1];
        const uint8_t spriteX = spriteXMinus8 - 8;
        uint8_t tileNumber = m_oam[i + 2];

        if (spriteHeight == 16) {
            // In 8x16 mode, the lower bit of the tile number is ignored.
            tileNumber &= 0xFE;
        }

        // Each tile is 16 bytes
        const uint16_t absoluteTileIndex = tileNumber * 16;

        // We need to find the our current line in the tile
        bool yFlip = (flags >> 6) & 1u;
        const uint8_t tileYOffset = yFlip ?
                                    ((spriteHeight - 1) - (m_currentY - spriteY)) :
                                    (m_currentY - spriteY);

        const uint16_t rowIndex = absoluteTileIndex + tileYOffset * 2;
        const uint8_t rowLow = m_vram[rowIndex];
        const uint8_t rowHigh = m_vram[static_cast<uint16_t>(rowIndex + 1)];

        // Loop through the row
        for (int x = 0; x < 8; ++x) {
            int pixelX = spriteX + x;

            // Is the pixel on screen?
            if (pixelX < 0 || pixelX >= 160) continue;

            const bool xFlip = (flags >> 5) & 1u;
            const uint8_t pos = xFlip ? x : (7 - x);

            const uint8_t pixelLow = ((rowLow >> pos) & 1u) ? 1 : 0;
            const uint8_t pixelHigh = ((rowHigh >> pos) & 1u) ? 2 : 0;
            const uint8_t pixel = pixelLow + pixelHigh;

            const bool usePallete0 = (flags >> 4) & 1u;
            const Colour colour = usePallete0 ?
                                  getPaletteColour(m_spritePalette0, pixel) :
                                  getPaletteColour(m_spritePalette1, pixel);

            // If the pixel is not transparent
            if (pixel != 0) {
                const uint16_t index = m_currentY * 160 + pixelX;

                // Render above BG?
                const bool hasPriority0 = ((flags >> 7u) & 1u) == 0;
                const bool bgPixelIsEmpty = m_frameBuffer[index] == getPaletteColour(m_bgPalette, 0);
                if (hasPriority0 || bgPixelIsEmpty) {
                    m_frameBuffer[index] = colour;
                }
            }
        }
    }
}

bool GPU::switchMode(GPUMode newMode) {
    // Set the lower 2 bits of STAT to newMode
    m_status &= ~0b11u;
    m_status |= static_cast<uint8_t>(newMode);

    // Should we request a STAT interrupt?
    switch (newMode) {
        case GPUMode::HORIZONTAL_BLANK: return statInterruptEnabled(StatInterrupt::HBLANK);
        case GPUMode::VERTICAL_BLANK:   return statInterruptEnabled(StatInterrupt::VBLANK);
        case GPUMode::SCANLINE_OAM:     return statInterruptEnabled(StatInterrupt::OAM);
        default:                        return false;
    }
}

Colour GPU::getPaletteColour(uint8_t palette, uint8_t index) const {
    const uint8_t value = (palette >> (index * 2)) & 0b11u;
    switch (value) {
        case 0: return COLOUR_100; // White (off)
        case 1: return COLOUR_66;  // Light grey (33% on)
        case 2: return COLOUR_33;  // Dark grey (66% on)
        case 3: return COLOUR_0;   // Black (on)
        default:
            // Unreachable!
            std::cerr << "unreachable: GPU::getPalletteColour was passed an out-of-bounds index.\n";
            exit(2);
    }
}
