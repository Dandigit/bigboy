#ifndef BIGBOY_TILESET_H
#define BIGBOY_TILESET_H

#include <array>

enum class TileValue : uint8_t {
    ZERO = 0b00,
    ONE = 0b01,
    TWO = 0b10,
    THREE = 0b11
};

using Tile = std::array<std::array<TileValue, 8>, 8>;
using TileSet = std::array<Tile, 384>;

#endif //BIGBOY_TILESET_H
