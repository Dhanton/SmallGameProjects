#include "finalboss.h"

FinalBoss::FinalBoss(Context& context, TextureID id):
    Boss(context, id),
    maxVel(120.0f)
{
    m_life = 30;

    m_animation.addAnimation("MAIN", 11, 1, sf::Time::Zero, true);
    m_animation.getSprite().setScale(4.0f, 4.0f);

    m_animation.playAnimation("MAIN");

    setVelocity({maxVel, maxVel});

    m_newRespawnTime = sf::seconds(5.0f);

    setPosition(32.0f, 32.0f);
}

void FinalBoss::update(sf::Time eTime, EnemyManager &enemyMan, BulletManager &bMan, MineManager& mineMan)
{
    if (m_dead) return;

    m_shootTime += eTime;
    if (m_life <= 10) m_newRespawnTime += eTime;

    Boss::update(eTime, enemyMan, bMan, mineMan);

    if (m_life <= 10 && m_newRespawnTime >= sf::seconds(4.0f))
    {
        m_newRespawnTime -= sf::seconds(4.0f);

        enemyMan.addEnemy({0.0f, 0.0f});
        enemyMan.addEnemy({200.0f, 250.0f});
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
    else if (getPosition().y + getGlobalBounds().height >= 14.0f * 16.0f)
    {
        setVelocity({getVelocity().x, -getVelocity().y});
    }

    if (m_shootTime >= sf::seconds(4.0f))
    {
        bMan.addBullet(Direction::RIGHT, {getPosition().x + m_animation.getSprite().getGlobalBounds().width / 2.0f, getPosition().y + m_animation.getSprite().getGlobalBounds().height / 2.0f}, true, true);
        bMan.addBullet(Direction::LEFT, {getPosition().x + m_animation.getSprite().getGlobalBounds().width / 2.0f, getPosition().y + m_animation.getSprite().getGlobalBounds().height / 2.0f}, false, true);
        bMan.addBullet(Direction::UP, {getPosition().x + m_animation.getSprite().getGlobalBounds().width / 2.0f, getPosition().y + m_animation.getSprite().getGlobalBounds().height / 2.0f}, false, true);
        bMan.addBullet(Direction::DOWN, {getPosition().x + m_animation.getSprite().getGlobalBounds().width / 2.0f, getPosition().y + m_animation.getSprite().getGlobalBounds().height / 2.0f}, false, true);
        m_shootTime -= sf::seconds(4.0f);
    }

    m_animation.update(eTime);
}

void FinalBoss::respawn()
{
    Boss::respawn();

    m_life = 30;

    setVelocity({maxVel, maxVel});

    setPosition(32.0f, 32.0f);
}
