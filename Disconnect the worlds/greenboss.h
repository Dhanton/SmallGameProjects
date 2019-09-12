#ifndef GREENBOSS_H
#define GREENBOSS_H

#include <list>

#include "boss.h"

class GreenBoss : public Boss
{
public:
    GreenBoss(Context& context, TextureID id);

    void update(sf::Time eTime, EnemyManager& enemyMan, BulletManager &bMan, MineManager &mineMan);

    virtual sf::FloatRect getGlobalBounds();

    void respawn();

private:
    const float maxVel;

    sf::Time m_shootTime;
    sf::Time m_newRespawnTime;

    /* Please don't kill me for this */
    bool m_velChanged_1;
    bool m_velChanged_2;
};

#endif // GREENBOSS_H
