#ifndef BULLET_H
#define BULLET_H

#include "entity.h"
#include "direction.h"

class Bullet : public Entity
{
public:
    typedef std::unique_ptr<Bullet> Ptr;

public:
    Bullet(Direction dir, const sf::Texture& texture, bool isEnemy = false);

    void update(sf::Time eTime, const ObstacleManager &obsMan);
    void update(sf::Time eTime);

    sf::FloatRect getGlobalBounds() const;

    bool isDead() const;
    bool isEnemy() const;

    void destroy();

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Sprite m_sprite;

    bool m_dead;
    bool m_isEnemy;

    const float maxVel;

    sf::Time m_elapsed;
};

#endif // BULLET_H
