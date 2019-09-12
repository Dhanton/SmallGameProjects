#include "tilemap.h"

#include "mathhelper.h"

TileMap::TileMap(size_t width, size_t height, size_t size, const sf::Texture *texture):
    m_tileset(texture),
    m_size(size),
    m_map(width, height, 0)
{
    assert(texture != nullptr && size >= 1);
}

void TileMap::setTile(size_t i, size_t j, Tile_id id)
{
    m_map.at(i, j) = id;
}

bool TileMap::isColliding(sf::FloatRect rect) const
{
    sf::Rect<size_t> collisionSubMatrix = getSubMatrix(rect);

    for (size_t j = collisionSubMatrix.top; j < collisionSubMatrix.height; ++j)
        for (size_t i = collisionSubMatrix.left; i < collisionSubMatrix.width; ++i) {
            if (m_map.at(i, j) != 0)
                return true;
        }

    return false;
}

sf::Rect<size_t> TileMap::getSubMatrix(sf::FloatRect rect) const
{
    rect.left -= getPosition().x;
    rect.top  -= getPosition().y;

    return m_map.sub_matrix(rect, m_size);
}

sf::FloatRect TileMap::getRect() const
{
    return {getPosition().x, getPosition().y, static_cast<float>(m_map.size().x * m_size), static_cast<float>(m_map.size().y * m_size)};
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
       sf::Rect<size_t> subMatrix = getSubMatrix(Helper::viewToRect(target.getView()));

       sf::VertexArray array(sf::Quads);

       for (size_t j = subMatrix.top; j < subMatrix.height; ++j)
           for (size_t i = subMatrix.left; i < subMatrix.width; ++i) {
               size_t id = m_map.at(i, j);

               if (id == 0) continue; // Void tile

               sf::Vertex quad[4];

               quad[0].position = {static_cast<float>((i + 0) * m_size), static_cast<float>((j + 0) * m_size)};
               quad[1].position = {static_cast<float>((i + 1) * m_size), static_cast<float>((j + 0) * m_size)};
               quad[2].position = {static_cast<float>((i + 1) * m_size), static_cast<float>((j + 1) * m_size)};
               quad[3].position = {static_cast<float>((i + 0) * m_size), static_cast<float>((j + 1) * m_size)};

               size_t textureWidth = m_tileset->getSize().x / m_size;

               quad[0].texCoords = {static_cast<float>((id - 1) % textureWidth), static_cast<float>(std::round((id - 1) / textureWidth))};
               quad[1].texCoords = {static_cast<float>((id - 1) % textureWidth + m_size), static_cast<float>(std::round((id - 1) / textureWidth))};
               quad[2].texCoords = {static_cast<float>((id - 1) % textureWidth + m_size), static_cast<float>(std::round((id - 1) / textureWidth) + m_size)};
               quad[3].texCoords = {static_cast<float>((id - 1) % textureWidth), static_cast<float>(std::round((id - 1) / textureWidth) + m_size)};

               for (size_t i = 0; i < 4; ++i) array.append(quad[i]);
           }

       states.texture = m_tileset;
       states.transform = getTransform();

       target.draw(array, states);
}
