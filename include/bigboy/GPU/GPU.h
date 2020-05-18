#ifndef BIGBOY_GPU_H
#define BIGBOY_GPU_H

#include <bigboy/GPU/TileSet.h>
#include <bigboy/MMU/MemoryDevice.h>

enum class GPUMode {
    HORIZONTAL_BLANK = 0, // 204 cycles
    VERTICAL_BLANK = 1, // 4560 cycles
    SCANLINE_OAM = 2, // 80 cycles
    SCANLINE_VRAM = 3 // 172 cycles
};

enum class Pixel : uint8_t {
    OFF = 0,
    LIGHT_GREY = 1,
    DARK_GREY = 2,
    BLACK = 3
};

class GPU : public MemoryDevice {
    // VRAM: 8000-9FFF
    std::array<uint8_t, 0x1FFF + 1> m_vram{0};

    // OAM: FE00-FE9F
    std::array<uint8_t, 0x9FFF + 1> m_oam{0};

    // I/O registers: FF40-FF4B
    uint8_t m_control; // FF40
    bool displayEnable() const { return (m_control >> 7u) & 1u; }
    bool windowTileset() const { return (m_control >> 6u) & 1u; };
    bool windowEnable()  const { return (m_control >> 5u) & 1u; };
    bool tileMap()       const { return (m_control >> 4u) & 1u; };
    bool bgTileset()     const { return (m_control >> 3u) & 1u; };
    bool spriteSize()    const { return (m_control >> 2u) & 1u; };
    bool spriteEnable()  const { return (m_control >> 1u) & 1u; };
    bool bgEnable()      const { return m_control         & 1u; };

    uint8_t m_status; // FF41

    uint8_t m_scrollY;  // FF42
    uint8_t m_scrollX;  // FF43

    uint8_t m_currentY;   // FF44
    uint8_t m_currentYCompare;  // FF45

    uint8_t m_windowY;   // FF4A
    uint8_t m_windowX;   // FF4B

    uint8_t m_bgPalette;  // FF47
    Pixel getPaletteColour(uint8_t index) const {
        return static_cast<Pixel>(
                (m_bgPalette >> static_cast<uint8_t>(index)) & 0b11u);
    };

    uint8_t m_spritePalette0; // FF48
    uint8_t m_spritePalette1; // FF49

    uint8_t m_dma;  // FF46

    // Cached, processed VRAM data
    //TileSet m_tileset{TileValue::ZERO}; // from 8000-8FFF

    std::array<Pixel, 160*144> m_bgBuffer{Pixel::OFF};
    std::array<Pixel, 160*144> m_frameBuffer{Pixel::OFF};

    // The kind of work we are (supposedly) doing
    GPUMode m_mode = GPUMode::HORIZONTAL_BLANK;

    // Keep track of how long it has taken us to do this work
    // Once we have had enough time to (supposedly) get it done,
    // we switch to the next mode.
    uint64_t m_clock = 0;

    // Keep track of the current blanking line. A complete horizontal
    // blank will take 143 lines, followed by 10 consecutive lines for
    // the following complete vertical blank. Afterwards, this value
    // is reset back to 0.
    // uint8_t m_currentLine = 0;

    // Reset the clock and switch to the new mode
    void switchMode(GPUMode newMode);
public:
    GPU() = default;

    // Get the current framebuffer
    const std::array<Pixel, 160*144>& getCurrentFrame() const;

    // Returns true if the GPU has finished rendering a frame, false otherwise
    bool step(uint8_t cycles);

    // Render one scanline into the framebuffer
    void renderScanline();

    void renderBackgroundScanline();

    // Update the tileset from a given VRAM address
    //void updateTileset(uint16_t address);

    void reset();

    std::vector<AddressSpace> addressSpaces() const override;
    uint8_t readByte(uint16_t address) const override;
    void writeByte(uint16_t address, uint8_t value) override;
};

#endif //BIGBOY_GPU_H
