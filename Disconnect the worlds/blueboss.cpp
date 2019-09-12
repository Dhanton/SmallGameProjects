#include "blueboss.h"

#include "mathhelper.h"

BlueBoss::BlueBoss(Context &context, TextureID id):
    Boss(context, id),
    maxVel(120.0f)
{
    m_life = 30;

    m_animation.addAnimation("MAIN", 25, 1, sf::Time::Zero, true);
    m_animation.getSprite().setScale(4.0f, 4.0f);

    m_animation.playAnimation("MAIN");

    setVelocity({-maxVel, -maxVel});

    m_newRespawnTime = sf::seconds(5.0f);

    setPosition(100.0f, 100.0f);
}

void BlueBoss::update(sf::Time eTime, EnemyManager& enemyMan, BulletManager& bulletMan, MineManager &mineMan)
{
    if (m_dead) return;

    if (m_life <= 15) m_newRespawnTime += eTime;

    Boss::update(eTime, enemyMan, bulletMan, mineMan);

    if (m_life <= 15 && m_newRespawnTime >= sf::seconds(5.0f))
    {
        m_newRespawnTime -= sf::seconds(5.0f);

        enemyMan.addEnemy({Helper::randRange(30.0f, 200.0f), 0.0f});
        enemyMan.addEnemy({Helper::randRange(30.0f, 200.0f), 250.0f});
    }

    if (getPosition().x <= 16.0f)
    {
        setVelocity({-getVelocity().x, getVelocity().y});
    }
    if (getPosition().y <= 25.0f)
    {
        setVelocity({getVelocity().x, -getVelocity().y});
    }
    if (getPosition().x + getGlobalBounds().width >= 20.0f * 16.0f)
    {
        setVelocity({-getVelocity().x, getVelocity().y});
    }
    if (getPosition().y + getGlobalBounds().height >= 14.0f * 16.0f)
    {
        setVelocity({getVelocity().x, -getVelocity().y});
    }

    m_animation.update(eTime);
}

void BlueBoss::respawn()
{
    Boss::respawn();

    m_life = 30;

    setVelocity({-maxVel, -maxVel});

    setPosition(100.0f, 100.0f);
}
