#ifndef TILEMAP_H
#define TILEMAP_H

#include "tileset.h"

enum class Tile
{
    VOID,
    GRASS,
    BORDER_DOWN,
    BORDER_LEFT,
    BORDER_RIGHT,
    BORDER_CORNER_1,
    BORDER_CORNER_2,
    WATER,
    DESERT,
    DESERT_CORNER_1,
    DESERT_CORNER_2,
    DESERT_CORNER_3,
    RAMP
};

class TileMap : public TileSet
{
public:
    TileMap(Context& context, TextureID id);

    void loadFromImage(const std::string& filename);

    Tile getTile(sf::Vector2f pos) const;

private:
    void addTile(const sf::Vector2i pos, Tile type);

    sf::Vector2u toTexturePos(Tile type);
    Tile         toTile(sf::Color c);

private:
    std::vector<std::vector<Tile>> m_tiles;

    const float m_size;
};

#endif // TILEMAP_H
