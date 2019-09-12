#ifndef HELL_H
#define HELL_H

#include "world.h"
#include "door.h"

class Hell : public World
{
public:
    Hell(Context& context);

    void update(sf::Time eTime, Game* game);

    void restart();

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void respawnEnemies();

private:
    SoundPlayer<>* m_soundPlayer;

    EnemyManager     m_enemyMan;
    ExplosionManager m_explosionMan;

    std::vector<Door::Ptr> m_doors;
};

#endif // HELL_H
