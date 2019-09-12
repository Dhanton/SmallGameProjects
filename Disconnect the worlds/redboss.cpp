#include "redboss.h"

#include "mathhelper.h"

RedBoss::RedBoss(Context &context, TextureID id):
    Boss(context, id),
    maxVel(150.0f)
{
    m_animation.addAnimation("MAIN", 35, 1, sf::Time::Zero, true);
    m_animation.getSprite().setScale(2.0f, 2.0f);

    m_animation.playAnimation("MAIN");

    m_dir = Direction::RIGHT;

    handleDirection();

    setPosition(50.0f, 50.0f);

    m_life = 30;
}

void RedBoss::update(sf::Time eTime, EnemyManager &enemyMan, BulletManager &bMan, MineManager& mineMan)
{
    if (m_dead) return;

    m_mineTime += eTime;

    if (m_mineTime >= sf::seconds(1.0f))
    {
        m_mineTime -= sf::seconds(1.0f);
        mineMan.addMine({Helper::randRange(50.0f, 300.0f), Helper::randRange(50.0f, 200.0f)});
    }

    if (getPosition().x < 16.0f)
    {
        setPosition({16.0f, getPosition().y});
        setRandomDirection();
    }

    if (getPosition().x + getGlobalBounds().width > 20.0f * 16.0f)
    {
        setPosition({20.0f * 16.0f - getGlobalBounds().width, getPosition().y});
        setRandomDirection();
    }

    if (getPosition().y < 25.0f)
    {
        setPosition({getPosition().x, 25.0f});
        setRandomDirection();
    }

    if (getPosition().y + getGlobalBounds().height > 14.0f * 16.0f)
    {
        setPosition({getPosition().x, 14.0f * 16.0f - getGlobalBounds().height});
        setRandomDirection();
    }

    Boss::update(eTime, enemyMan, bMan, mineMan);

    m_animation.update(eTime);
}

void RedBoss::handleDirection()
{
    switch (m_dir)
    {
    case Direction::RIGHT:
        setVelocity({maxVel, 0.0f});
        break;
    case Direction::LEFT:
        setVelocity({-maxVel, 0.0f});
        break;
    case Direction::UP:
        setVelocity({0.0f, -maxVel});
        break;
    case Direction::DOWN:
        setVelocity({0.0f, maxVel});
        break;
    }
}

void RedBoss::setRandomDirection()
{
    int dir = Helper::randRange(0, 2);

    if (m_dir == Direction::RIGHT || m_dir == Direction::LEFT)
    {
        if (dir == 0) m_dir = Direction::DOWN;
        else m_dir = Direction::UP;
    }
    else
    {
        if (dir == 0) m_dir = Direction::RIGHT;
        else m_dir = Direction::LEFT;
    }

    handleDirection();
}

void RedBoss::respawn()
{
    Boss::respawn();

    m_life = 30;

    m_dir = Direction::RIGHT;
    handleDirection();

    setPosition(50.0f, 50.0f);
}
