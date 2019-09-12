#include "map.hpp"
#include "entitymanager.hpp"

#include <iostream>

Map::Map(TextureLoader *tex)
{
    m_tex = tex;

    m_renderNormal = false;

    m_generation = 0;
}

bool Map::isColliding(sf::FloatRect rect)
{
    Vector2 pos = {rect.left, rect.top};
    Vector2 size = {rect.width, rect.height};

    int startX = static_cast<int>(pos.x/m_size);
    int endX = static_cast<int>((pos.x + size.x)/m_size);
    int startY = static_cast<int>(pos.y/m_size);
    int endY = static_cast<int>((pos.y + size.y)/m_size);

    for (int i = std::max(startX, 0); i <= std::min(endX, (int)m_map.size() - 1); ++i) {
        for (int j = std::max(startY, 0); j <= std::min(endY, (int)m_map[i].size() - 1); ++j) {
            if (m_map[i][j].solid) return true;
        }
    }

    return false;
}

void Map::populate(const std::string &file)
{
    sf::Image image;
    image.loadFromFile(file);

    sf::Vector2u size = image.getSize();

    for (size_t i = 0; i < size.x; ++i) {
        m_map.push_back(std::vector<Tile>());


        for (size_t j = 0; j < size.y; ++j) {
            sf::Color pixel = image.getPixel(i, j);

            Tile tile;

            if (pixel == sf::Color::Red) {
                tile.id = 0;
                tile.solid = true;
            }

            if (pixel == sf::Color::Black) {
                tile.id = 1;
                tile.solid = false;
            }

            m_map[i].push_back(tile);
        }
    }
}

void Map::generateRandomly(sf::Vector2u size, size_t maxNum)
{
    m_map.clear();

    for (size_t i = 0; i < size.x; ++i) {
        m_map.push_back(std::vector<Tile>());

        for (size_t j = 0; j < size.y; ++j) {
            Tile tile;
            tile.id = 0;
            tile.solid = true;

            m_map[i].push_back(tile);
        }
    }

    m_startingTile = {static_cast<unsigned int>(rand()%size.x), static_cast<unsigned int>(rand()%size.y)};
    m_startingTile.x = clamp(m_startingTile.x, size.x - 2, 1.f);
    m_startingTile.y = clamp(m_startingTile.y, size.y - 2, 1.f);

    sf::Vector2u pos = m_startingTile;

    m_startingPos = {static_cast<float>(m_startingTile.x * m_size - m_size/2.f + 10), static_cast<float>(m_startingTile.y * m_size - m_size/2.f + 10)};

    size_t maxWall = maxNum;
    size_t steps = 0;
    int direction = 0;

    m_map[pos.x][pos.y].id = 2;
    m_map[pos.x][pos.y].solid = false;

    while (steps < maxWall) {
        int newDir = rand()%4;

        if (newDir == -direction) {
            newDir = -newDir;
        }

        direction = newDir;

        int dirX = 0;
        int dirY = 0;

        if (direction == 0) {
            dirX = 1;

        } else if (direction == 1) {
            dirX = -1;

        } else if (direction == 2) {
            dirY = 1;

        } else if (direction == 3) {
            dirY = -1;
        }

        pos.x = clamp(pos.x + dirX, size.x - 2, 1);
        pos.y = clamp(pos.y + dirY, size.y - 2, 1);

        if (m_map[pos.x][pos.y].id != 2) {
            m_map[pos.x][pos.y].id = 2;
            m_map[pos.x][pos.y].solid = false;

            ++steps;

            if (steps >= maxWall) {
                m_map[pos.x][pos.y].id = 3;
                m_map[pos.x][pos.y].solid = false;
            }
        }
    }

    //Save to png file, restart at each death and add monsters
    sf::Image image;
    float scale = 2.f;

    image.create(size.x * scale, size.y * scale);

    for (size_t i = 0; i < m_map.size(); ++i) {
        for (size_t j = 0; j < m_map[i].size(); ++j) {
            sf::Color color = sf::Color::Blue;

            Tile& tile = m_map[i][j];

            if (tile.id == 3) color = sf::Color::Green;
            if (tile.id == 2) color = sf::Color::White;

            image.setPixel(i * scale, j * scale, color);
            image.setPixel(i * scale + 1, j * scale, color);
            image.setPixel(i * scale + 1, j * scale + 1, color);
            image.setPixel(i * scale, j * scale + 1, color);

        }
    }

    image.saveToFile("../generated_levels/level_" + std::to_string(m_generation) + ".png");

    ++m_generation;
}

void Map::spawnEnemies(EntityManager *manager) const
{
    for (size_t i = 0; i < m_map.size(); ++i) {
        for (size_t j = 0; j < m_map[i].size(); ++j) {
            const Tile& tile = m_map[i][j];

            if (std::abs(m_startingTile.x - i) <= 2 && std::abs(m_startingTile.y - j) <= 2)
                continue;

            if (tile.id == 2 || tile.id == 3) {
                Vector2 pos = {i * m_size, j * m_size};

                if (rand()%120 == 1) {
                    manager->addEntity(Entity::MONSTER, pos);
                }

                if (rand()%50 == 1) {
                    manager->addEntity(Entity::WORM, pos);
                }
            }
        }
    }
}

Tile &Map::getTile(size_t i, size_t j)
{
    return m_map[i][j];
}

const Tile &Map::getTile(size_t i, size_t j) const
{
    return m_map[i][j];
}

Vector2 Map::getStartingPos() const
{
    return m_startingPos;
}

float Map::getSize() const
{
    return m_size;
}

Vector2 Map::totalMapSize() const
{
    if (m_map.empty()) return Vector2();

    return {m_size * m_map.size(), m_size * m_map[0].size()};
}

void Map::renderNormal(bool normal)
{
    m_renderNormal = normal;
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    Vector2 center = target.getView().getCenter();
    Vector2 size = target.getView().getSize();

    int startX = static_cast<int>((center.x - size.x/2.f)/m_size);
    int endX = static_cast<int>((center.x + size.x/2.f)/m_size);
    int startY = static_cast<int>((center.y - size.y/2.f)/m_size);
    int endY = static_cast<int>((center.y + size.y/2.f)/m_size);

    for (int i = std::max(startX, 0); i <= std::min(endX, (int)m_map.size() - 1); ++i) {
        for (int j = std::max(startY, 0); j <= std::min(endY, (int)m_map[i].size() - 1); ++j) {
            const Tile& tile = m_map[i][j];

            sf::Sprite sprite;
            sprite.scale(5.f, 5.f);
            sprite.setTexture(m_renderNormal ? m_tex->getResource("NORMAL_TILESET") : m_tex->getResource("TILESET"));
            sprite.setTextureRect({tile.id * 16, 0, 16, 16});
            sprite.setPosition(i * m_size, j * m_size);

            target.draw(sprite, states);
        }
    }
}
