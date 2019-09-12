#ifndef OBSTACLEMANGER_H
#define OBSTACLEMANGER_H

#include "tileset.h"

enum class Obstacle
{
    ROCK,
    HELL_TREE,
    TREE
};

class ObstacleManager : public TileSet
{
public:
    ObstacleManager(Context& context, TextureID id);

    void addObstacle(Obstacle type, sf::Vector2f worldPos);

    void loadFromFile(const std::string& filename, bool hell = false);

    bool isColliding(sf::FloatRect rect) const;

private:
    sf::Vector2f getObstacleSize(Obstacle obs);
    sf::Vector2f getSpriteSize(Obstacle obs);

    sf::Vector2u toTexturePos(Obstacle type);

private:
    std::vector<sf::FloatRect> m_obstacles;
};

#endif // OBSTACLEMANGER_H
