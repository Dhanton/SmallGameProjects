#include "bullet.h"

#include <iostream>

Bullet::Bullet(Direction dir, const sf::Texture &texture, bool isEnemy):
    maxVel(200.0f)
{
    m_dead = false;

    m_isEnemy = isEnemy;

    m_sprite.setTexture(texture);
    m_sprite.setTextureRect({0, 4 * 16, 16, 10});

    m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2.0f,
                       m_sprite.getGlobalBounds().height / 2.0f);

    switch (dir)
    {
    case Direction::RIGHT:
        setVelocity({maxVel, 0.0f});
        break;
    case Direction::LEFT:
        setVelocity({-maxVel, 0.0f});
        m_sprite.rotate(180.0f);
        break;
    case Direction::DOWN:
        setVelocity({0.0f, maxVel});
        m_sprite.rotate(90.0f);
        break;
    case Direction::UP:
        setVelocity({0.0f, -maxVel});
        m_sprite.rotate(-90.0f);
        break;
    }
}

void Bullet::update(sf::Time eTime, const ObstacleManager &obsMan)
{
    if (m_dead) return;

    m_elapsed += eTime;

    Entity::update(eTime);

    if (obsMan.isColliding(getGlobalBounds())){
        setVelocity({0.0f, 0.0f});
    }

    if (m_elapsed >= sf::seconds(2.0f)) m_dead = true;
}

void Bullet::update(sf::Time eTime)
{
    if (m_dead) return;

    m_elapsed += eTime;

    Entity::update(eTime);

    if (m_elapsed >= sf::seconds(2.0f)) m_dead = true;
}

sf::FloatRect Bullet::getGlobalBounds() const
{
    return getTransform().transformRect(m_sprite.getGlobalBounds());
}

bool Bullet::isDead() const
{
    return m_dead;
}

bool Bullet::isEnemy() const
{
    return m_isEnemy;
}

void Bullet::destroy()
{
    m_dead = true;
}

void Bullet::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_sprite, states);
}
