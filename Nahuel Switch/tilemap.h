#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include <cassert>
#include <vector>

#include "matrix.h"
#include "player.h"

namespace
{
using Tile_id = size_t;
}

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap(size_t width, size_t height, size_t size, const sf::Texture* texture);

    void setTile(size_t i, size_t j, Tile_id id);

    bool isColliding(sf::FloatRect rect) const;

    sf::Rect<size_t> getSubMatrix(sf::FloatRect rect) const;

    sf::FloatRect getRect() const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    const sf::Texture* m_tileset;
    const size_t       m_size;

    Matrix<Tile_id> m_map;
};

#endif // TILEMAP_H
