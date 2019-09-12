#ifndef FINALBOSS_H
#define FINALBOSS_H

#include "boss.h"

class FinalBoss : public Boss
{
public:
    FinalBoss(Context& context, TextureID id);

    void update(sf::Time eTime, EnemyManager &enemyMan, BulletManager &bMan, MineManager &mineMan);

    void respawn();

private:
    const float maxVel;

    sf::Time m_shootTime;
    sf::Time m_newRespawnTime;
};

#endif // FINALBOSS_H
