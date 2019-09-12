#include "tilemap.h"

#include "mathhelper.h"

TileMap::TileMap(Context& context, TextureID id):
    TileSet(context, id),
    m_size(16.0f)
{

}

void TileMap::loadFromImage(const std::string &filename)
{
    sf::Image image;

    if (!image.loadFromFile(filename)) return;

    m_tiles = {image.getSize().x, {image.getSize().y, Tile::VOID}};

    for (unsigned int i = 0; i < image.getSize().x; ++i)
        for (unsigned int j = 0; j < image.getSize().y; ++j)
        {
            sf::Color c = image.getPixel(i, j);

            Tile tile = toTile(c);

            m_tiles.at(i).at(j) = tile;

            addTile({i, j}, tile);
        }
}

void TileMap::addTile(const sf::Vector2i pos, Tile type)
{
    sf::Vector2f worldPos = {(float) pos.x * m_size, (float) pos.y * m_size};

    sf::Vector2u textPos = toTexturePos(type);

    addVertex(worldPos.x, worldPos.y, textPos.x, textPos.y);
    addVertex(worldPos.x + m_size, worldPos.y, textPos.x + m_size, textPos.y);
    addVertex(worldPos.x + m_size, worldPos.y + m_size, textPos.x + m_size, textPos.y + m_size);
    addVertex(worldPos.x, worldPos.y + m_size, textPos.x, textPos.y + m_size);
}

sf::Vector2u TileMap::toTexturePos(Tile type)
{
    switch (type)
    {
    case Tile::GRASS:
        return {Helper::randRange(1, 3, true) * (unsigned int) m_size, 0};
    case Tile::WATER:
        return {Helper::randRange(1, 3, true) * (unsigned int) m_size, (unsigned int) m_size};
    case Tile::BORDER_DOWN:
        return {Helper::randRange(5, 6, true) * (unsigned int) m_size, (unsigned int) m_size};
    case Tile::BORDER_RIGHT:
        return {Helper::randRange(6, 7, true) * (unsigned int) m_size, 0};
    case Tile::BORDER_LEFT:
        return {Helper::randRange(5, 6, true) * (unsigned int) m_size, 0};
    case Tile::BORDER_CORNER_1:
        return {4 * (unsigned int) m_size, (unsigned int) m_size};
    case Tile::BORDER_CORNER_2:
        return {7 * (unsigned int) m_size, (unsigned int) m_size};
    case Tile::DESERT:
        return {Helper::randRange(1, 3, true) * (unsigned int) m_size, 2 * (unsigned int) m_size};
    case Tile::DESERT_CORNER_1:
        return {4 * (unsigned int) m_size, 2 * (unsigned int) m_size};
    case Tile::DESERT_CORNER_2:
        return {5 * (unsigned int) m_size, 2 * (unsigned int) m_size};
    case Tile::DESERT_CORNER_3:
        return {6 * (unsigned int) m_size, 2 * (unsigned int) m_size};
    case Tile::RAMP:
        return {2 * (unsigned int) m_size, 3 * (unsigned int) m_size};
    default:
        return {};
    }
}

Tile TileMap::toTile(sf::Color c)
{
    if (c == sf::Color::Green) return Tile::GRASS;
    if (c == sf::Color::Blue) return Tile::WATER;

    if (c.r == 255 && c.g == 0)
    {
        if (c.b == 0) return Tile::BORDER_DOWN;
        if (c.b == 50) return Tile::BORDER_RIGHT;
        if (c.b == 100) return Tile::BORDER_LEFT;
        if (c.b == 150) return Tile::BORDER_CORNER_1;
        if (c.b == 200) return Tile::BORDER_CORNER_2;
    }

    if (c.b == 255 && c.g == 0)
    {
        if (c.r == 50) return Tile::DESERT;
        if (c.r == 100) return Tile::DESERT_CORNER_1;
        if (c.r == 150) return Tile::DESERT_CORNER_2;
        if (c.r == 200) return Tile::DESERT_CORNER_3;
    }

    if (c == sf::Color::White) return Tile::RAMP;
}

Tile TileMap::getTile(sf::Vector2f pos) const
{
    int x = static_cast<int>(pos.x / m_size);
    int y = static_cast<int>(pos.y / m_size);

    return m_tiles.at(x).at(y);
}
