#ifndef BOSS_H
#define BOSS_H

#include <SFML/Graphics.hpp>

#include "animationmanager.h"
#include "context.h"
#include "entity.h"
#include "enemymanager.h"
#include "minemanager.h"

class Boss : public Entity
{
public:
    typedef std::unique_ptr<Boss> Ptr;

public:
    Boss(Context& context, TextureID id);

    virtual void update(sf::Time eTime, EnemyManager& enemyMan, BulletManager &bMan,
                        MineManager& mineMan);

    virtual sf::FloatRect getGlobalBounds();

    void takeDamage();
    virtual void respawn();

    bool isDead() const;

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

protected:
    AnimationManager m_animation;

    unsigned int m_life;

    bool m_dead;
};

#endif // BOSS_H
