#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include "defines.hpp"
#include "res_loader.hpp"

class EntityManager;

struct Tile {
    u8 id;
    bool solid;

    Tile()
    {
        id = 2;
        solid = false;
    }
};

class Map : public sf::Drawable
{
public:
    Map(TextureLoader* tex);

    bool isColliding(sf::FloatRect rect);
    void populate(const std::string& file);
    void generateRandomly(sf::Vector2u size, size_t maxNum);
    void spawnEnemies(EntityManager* manager) const;

    Tile& getTile(size_t i, size_t j);
    const Tile& getTile(size_t i, size_t j) const;

    Vector2 getStartingPos() const;

    float getSize() const;
    Vector2 totalMapSize() const;
    void renderNormal(bool normal);

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    TextureLoader* m_tex;

    const float m_size = 80.f;
    size_t m_generation;
    bool m_renderNormal;
    Vector2 m_startingPos;
    sf::Vector2u m_startingTile;
    std::vector<std::vector<Tile>> m_map;
};

#endif // MAP_HPP
