#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "animationmanager.h"
#include "bulletmanager.h"
#include "explosionmanager.h"
#include "player.h"

class Enemy : public Entity
{
public:
    typedef std::unique_ptr<Enemy> Ptr;

public:
    Enemy(const sf::Texture& texture);

    void update(sf::Time eTime, const ObstacleManager &obsMan, const TileMap &map,
                BulletManager& bMan, ExplosionManager& explMan, Player* player);

    void update(sf::Time eTime, BulletManager& bMan, ExplosionManager& explMan, Player* player);

    void takeDamage();

    sf::FloatRect getGlobalBounds();

    bool isDead() const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void setRandomDirection();
    void computeDirection();

    void followPlayer(Player* player);

    float getDistance(const sf::Vector2f& point);


private:
    bool m_dead;

    unsigned int m_lifes;

    AnimationManager m_animation;

    Direction m_dir;

    const float maxVel;
    const float minDist;

    sf::Time m_toNewDirection;

    bool m_isFollowing;
};

#endif // ENEMY_H
