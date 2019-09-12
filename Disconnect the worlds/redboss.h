#ifndef REDBOSS_H
#define REDBOSS_H

#include "boss.h"
#include "direction.h"

class RedBoss : public Boss
{
public:
    RedBoss(Context& context, TextureID id);

    void update(sf::Time eTime, EnemyManager &enemyMan, BulletManager &bMan, MineManager &mineMan);

    void respawn();

private:
    void handleDirection();
    void setRandomDirection();

private:
    const float maxVel;

    Direction m_dir;

    sf::Time m_mineTime;
};

#endif // REDBOSS_H
