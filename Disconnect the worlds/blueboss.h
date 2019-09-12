#ifndef BLUEBOSS_H
#define BLUEBOSS_H

#include "boss.h"

class BlueBoss : public Boss
{
public:
    BlueBoss(Context& context, TextureID id);

    void update(sf::Time eTime, EnemyManager& enemyMan, BulletManager &bulletMan,
                MineManager& mineMan);

    void respawn();

private:
    const float maxVel;

    sf::Time m_newRespawnTime;
};

#endif // BLUEBOSS_H
