#include "entity.h"

#include "mathhelper.h"

Entity::Entity()
{
    setOrigin(getGlobalBounds().width / 2.0f, getGlobalBounds().height / 2.0f);
}

void Entity::update(sf::Time eTime)
{
    m_velocity += m_acceleration;

    move(m_velocity * eTime.asSeconds());

    checkLimitCollision();
}

void Entity::update(sf::Time eTime, const ObstacleManager &obsMan, const TileMap &map)
{
    m_velocity += m_acceleration;

    moveColliding(eTime, obsMan, map);

    move(m_velocity * eTime.asSeconds());

    checkLimitCollision();
}

void Entity::update(sf::Time eTime, const ObstacleManager &obsMan)
{
    m_velocity += m_acceleration;

    moveColliding(eTime, obsMan);

    move(m_velocity * eTime.asSeconds());

    checkLimitCollision();
}

sf::FloatRect Entity::getGlobalBounds()
{
    return {};
}

Vector2 Entity::getVelocity() const
{
    return m_velocity;
}

void Entity::setVelocity(const Vector2 &velocity)
{
    m_velocity = velocity;
}

Vector2 Entity::getAcceleration() const
{
    return m_acceleration;
}

void Entity::setAcceleration(const Vector2 &acceleration)
{
    m_acceleration = acceleration;
}

void Entity::moveColliding(sf::Time eTime, const ObstacleManager &obsMan,
                           const TileMap& map)
{
    sf::FloatRect xRect = getGlobalBounds();
    sf::FloatRect yRect = getGlobalBounds();

    xRect.left += getVelocity().x * eTime.asSeconds();
    yRect.top += getVelocity().y * eTime.asSeconds();

    if (obsMan.isColliding(xRect) || map.getTile({xRect.left, xRect.top}) == Tile::WATER)
    {
        for (float i = 0.0f; i < Helper::abs(getVelocity().x); i += 0.1f)
        {
            if (!tryMove({0.1f * Helper::sgn(getVelocity().x), 0.0f}, obsMan, map)){
                setVelocity({0.0f, getVelocity().y});
            }
        }
    }

    if (obsMan.isColliding(yRect) || map.getTile({yRect.left, yRect.top}) == Tile::WATER)
    {
        for (float i = 0.0f; i < Helper::abs(getVelocity().y); i += 0.1f)
        {
            if (!tryMove({0.0f, 0.1f * Helper::sgn(getVelocity().y)}, obsMan, map)){
                setVelocity({getVelocity().x, 0.0f});
            }
        }
    }
}

bool Entity::tryMove(const Vector2 &vel, const ObstacleManager &obsMan,
                     const TileMap& map)
{
    sf::FloatRect rect = getGlobalBounds();
    rect.left += vel.x;
    rect.top += vel.y;

    if (!obsMan.isColliding(rect) && map.getTile({rect.left, rect.top}) != Tile::WATER)
    {
        move(vel);
        return true;
    }

    return false;
}

void Entity::moveColliding(sf::Time eTime, const ObstacleManager &obsMan)
{
    sf::FloatRect xRect = getGlobalBounds();
    sf::FloatRect yRect = getGlobalBounds();

    xRect.left += getVelocity().x * eTime.asSeconds();
    yRect.top += getVelocity().y * eTime.asSeconds();

    if (obsMan.isColliding(xRect))
    {
        for (float i = 0.0f; i < Helper::abs(getVelocity().x * eTime.asSeconds()); i += 0.1f)
        {
            if (!tryMove({0.1f * Helper::sgn(getVelocity().x), 0.0f}, obsMan)){
                setVelocity({0.0f, getVelocity().y});
            }
        }
    }

    if (obsMan.isColliding(yRect))
    {
        for (float i = 0.0f; i < Helper::abs(getVelocity().y * eTime.asSeconds()); i += 0.1f)
        {
            if (!tryMove({0.0f, 0.1f * Helper::sgn(getVelocity().y)}, obsMan)){
                setVelocity({getVelocity().x, 0.0f});
            }
        }
    }
}

bool Entity::tryMove(const Vector2 &vel, const ObstacleManager &obsMan)
{
    sf::FloatRect rect = getGlobalBounds();
    rect.left += vel.x;
    rect.top += vel.y;

    if (!obsMan.isColliding(rect))
    {
        move(vel);
        return true;
    }

    return false;
}

void Entity::checkLimitCollision()
{
    if (getPosition().x < 0.0f)
    {
        setPosition(0.0f, getPosition().y);
        setVelocity({0.0f, getVelocity().y});
    }
    if (getPosition().y < 0.0f)
    {
        setPosition(getPosition().x, 0.0f);
        setVelocity({getVelocity().x, 0.0f});
    }

    if (getPosition().x + getGlobalBounds().width >= 49.0f * 16.0f)
    {
        setPosition(49.0f * 16.0f - getGlobalBounds().width, getPosition().y);
        setVelocity({0.0f, getVelocity().y});
    }
    if (getPosition().y + getGlobalBounds().height >= 49.0f * 16.0f)
    {
        setPosition(getPosition().x, 49.0f * 16.0f - getGlobalBounds().height);
        setVelocity({getVelocity().x, 0.0f});
    }

}



