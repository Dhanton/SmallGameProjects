#include "obstaclemanager.h"

#include "mathhelper.h"

ObstacleManager::ObstacleManager(Context &context, TextureID id):
    TileSet(context, id)
{

}

void ObstacleManager::addObstacle(Obstacle type, sf::Vector2f worldPos)
{
    sf::FloatRect rect = {worldPos, getObstacleSize(type)};

    if (type == Obstacle::TREE)
    {
        rect.top += 9.0f;
        rect.left += 3.0f;
    }

    if (type == Obstacle::HELL_TREE)
    {
        rect.top += 12.0f;
    }

    m_obstacles.push_back(rect);

    sf::Vector2f size = getSpriteSize(type);

    sf::Vector2u textPos = toTexturePos(type);

    addVertex(worldPos.x,          worldPos.y, textPos.x, textPos.y);
    addVertex(worldPos.x + size.x, worldPos.y, textPos.x + size.x, textPos.y);
    addVertex(worldPos.x + size.x, worldPos.y + size.y, textPos.x + size.x, textPos.y + size.y);
    addVertex(worldPos.x,          worldPos.y + size.y, textPos.x, textPos.y + size.y);
}

void ObstacleManager::loadFromFile(const std::string &filename, bool hell)
{
    sf::Image image;

    if (!image.loadFromFile(filename)) return;

    for (unsigned int i = 0; i < image.getSize().x; ++i)
        for (unsigned int j = 0; j < image.getSize().y; ++j)
        {
            sf::Color c = image.getPixel(i, j);

            if (c == sf::Color::Red)
                addObstacle(Obstacle::ROCK, {static_cast<float>(i * 16), static_cast<float>(j * 16)});
            if (c == sf::Color::Blue)
            {
                if (hell) addObstacle(Obstacle::HELL_TREE, {static_cast<float>(i * 16), static_cast<float>(j * 16)});
                else addObstacle(Obstacle::TREE, {static_cast<float>(i * 16), static_cast<float>(j * 16)});
            }

        }
}

bool ObstacleManager::isColliding(sf::FloatRect rect) const
{
    for (auto& obs : m_obstacles)
    {
        if (rect.intersects(obs)) return true;
    }

    return false;
}

sf::Vector2f ObstacleManager::getObstacleSize(Obstacle obs)
{
    switch (obs)
    {
    case Obstacle::ROCK:
        return {16.0f, 16.0f};
    case Obstacle::HELL_TREE:
        return {14.0f, 15.0f};
    case Obstacle::TREE:
        return {8.0f, 15.0f};
    }
}

sf::Vector2f ObstacleManager::getSpriteSize(Obstacle obs)
{
    switch (obs)
    {
    case Obstacle::ROCK:
        return {16.0f, 16.0f};
    case Obstacle::HELL_TREE:
        return {16.0f, 27.0f};
    case Obstacle::TREE:
        return {14.0f, 24.0f};
    }
}

sf::Vector2u ObstacleManager::toTexturePos(Obstacle type)
{
    switch (type)
    {
    case Obstacle::ROCK:
        return {0, 16};
    case Obstacle::HELL_TREE:
        return {0, 32};
    case Obstacle::TREE:
        return {0, 32};
    }
}
