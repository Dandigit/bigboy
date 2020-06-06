#ifndef BIGBOY_GPU_H
#define BIGBOY_GPU_H

#include <bigboy/GPU/TileSet.h>
#include <bigboy/MMU/MemoryDevice.h>

enum class GPUMode {
    HORIZONTAL_BLANK = 0, // 204 cycles (H-Blank)
    VERTICAL_BLANK = 1, // 4560 cycles  (V-Blank)
    SCANLINE_OAM = 2, // 80 cycles      (Searching OAM-RAM)
    SCANLINE_VRAM = 3 // 172 cycles     (Transferring Data to LCD Driver)
};

enum class StatInterrupt : uint8_t {
    HBLANK = 3, // Horizontal blank, bit 3
    VBLANK = 4, // Vertical blank, bit 4
    OAM = 5,    // OAM read, bit 5
    LYC = 6,    // LY coincidence, bit 6
};

struct Colour {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

class GPU : public MemoryDevice {
public:
    struct Request {
        bool vblank;
        bool stat;
    };

    GPU() = default;

    // Returns true if the GPU has finished rendering a frame, false otherwise
    Request update(uint8_t cycles);

    // Get the current framebuffer
    const std::array<Colour, 160*144>& getCurrentFrame() const;

    void reset();

    std::vector<AddressSpace> addressSpaces() const override;
    uint8_t readByte(uint16_t address) const override;
    void writeByte(uint16_t address, uint8_t value) override;

private:
    // Render one scanline into the framebuffer
    void renderScanline();
    void renderBackgroundScanline();
    void renderSpriteScanline();

    GPUMode getMode() const { return static_cast<GPUMode>(m_status & 0b11u); }

    // Switch to the new mode; updating the STAT register accordingly
    // Returns true if a STAT interrupt is to be requested
    bool switchMode(GPUMode newMode);

    bool statInterruptEnabled(StatInterrupt interrupt) const
            { return (m_status >> static_cast<uint8_t>(interrupt)) & 1u; }

    void setCoincidenceFlag()   { m_status |= (1u << 2u); };
    void clearCoincidenceFlag() { m_status &= ~(1u << 2u); };

    bool displayEnable() const { return (m_control >> 7u) & 1u; }
    bool windowTileset() const { return (m_control >> 6u) & 1u; };
    bool windowEnable()  const { return (m_control >> 5u) & 1u; };
    bool tileMap()       const { return (m_control >> 4u) & 1u; };
    bool bgTileset()     const { return (m_control >> 3u) & 1u; };
    bool spriteSize()    const { return (m_control >> 2u) & 1u; };
    bool spriteEnable()  const { return (m_control >> 1u) & 1u; };
    bool bgEnable()      const { return m_control         & 1u; };

    Colour getPaletteColour(uint8_t palette, uint8_t index) const;

    // VRAM: 8000-9FFF
    std::array<uint8_t, 0x1FFF + 1> m_vram{0};

    // OAM: FE00-FE9F
    std::array<uint8_t, 0x9FFF + 1> m_oam{0};

    // I/O registers: FF40-FF4B
    uint8_t m_control; // FF40

    uint8_t m_status; // FF41

    uint8_t m_scrollY;  // FF42
    uint8_t m_scrollX;  // FF43

    uint8_t m_currentY;   // FF44
    uint8_t m_currentYCompare;  // FF45

    uint8_t m_windowY;   // FF4A
    uint8_t m_windowX;   // FF4B

    uint8_t m_bgPalette;  // FF47

    uint8_t m_spritePalette0; // FF48
    uint8_t m_spritePalette1; // FF49

    uint8_t m_dma;  // FF46

    std::array<Colour, 160*144> m_frameBuffer{Colour{0, 0, 0, 255}};

    // Keep track of how long it has taken us to do this work
    // Once we have had enough time to (supposedly) get it done,
    // we switch to the next mode.
    uint64_t m_clock = 0;
};

#endif //BIGBOY_GPU_H
