#ifndef BIGBOY_GPU_H
#define BIGBOY_GPU_H

#include <bigboy/GPU/TileSet.h>
#include <bigboy/MMU/MemoryDevice.h>

class GPU : public MemoryDevice {
    std::array<uint8_t, 0x1FFF + 1> m_vram{0};
    TileSet m_tileset{TileValue::ZERO};

public:
    GPU() = default;

    void updateTileset(uint16_t address, uint8_t value);

    std::vector<AddressSpace> addressSpaces() const override;
    uint8_t readByte(uint16_t address) const override;
    void writeByte(uint16_t address, uint8_t value) override;
};

#endif //BIGBOY_GPU_H
