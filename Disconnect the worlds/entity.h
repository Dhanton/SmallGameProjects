#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

#include "vector2.h"
#include "obstaclemanager.h"
#include "tilemap.h"

class Entity : public sf::Drawable, public sf::Transformable
{
public:
    Entity();

    virtual void update(sf::Time eTime);
    virtual void update(sf::Time eTime, const ObstacleManager& obsMan);
    virtual void update(sf::Time eTime, const ObstacleManager& obsMan,
                        const TileMap& map);

    virtual sf::FloatRect getGlobalBounds();

    Vector2 getVelocity() const;
    void    setVelocity(const Vector2 &velocity);

    Vector2 getAcceleration() const;
    void    setAcceleration(const Vector2 &acceleration);

private:
    void moveColliding(sf::Time eTime, const ObstacleManager& obsMan, const TileMap &map);
    void moveColliding(sf::Time eTime, const ObstacleManager& obsMan);

    bool tryMove(const Vector2& vel, const ObstacleManager& obsMan, const TileMap &map);
    bool tryMove(const Vector2& vel, const ObstacleManager& obsMan);

    void checkLimitCollision();

private:
    Vector2 m_velocity;
    Vector2 m_acceleration;


};

#endif // ENTITY_H
