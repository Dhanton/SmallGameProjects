#include "greenboss.h"

#include "mathhelper.h"

GreenBoss::GreenBoss(Context &context, TextureID id):
    Boss(context, id),
    maxVel(85.0f)
{
    m_life = 50;

    m_animation.addAnimation("MAIN", 45, 1, sf::Time::Zero, true);
    m_animation.getSprite().setScale(2.0f, 2.0f);

    m_animation.playAnimation("MAIN");

    setVelocity({maxVel, 0.0f});

    setPosition(50.0f, 35.0f);

    m_newRespawnTime = sf::seconds(7.0f);

    m_velChanged_1 = false;
    m_velChanged_2 = false;
}

void GreenBoss::update(sf::Time eTime, EnemyManager &enemyMan, BulletManager &bMan, MineManager& mineMan)
{
    if (m_dead) return;

    m_shootTime += eTime;
    if (m_life <= 20) m_newRespawnTime += eTime;

    Boss::update(eTime, enemyMan, bMan, mineMan);

    if (m_life <= 20 && m_newRespawnTime >= sf::seconds(7.0f))
    {
        m_newRespawnTime -= sf::seconds(7.0f);

        enemyMan.addEnemy({0.0f, 0.0f});
        enemyMan.addEnemy({200.0f, 250.0f});
    }

    if (m_life <= 40 && !m_velChanged_1)
    {
        setVelocity({(maxVel + 30) * Helper::sgn(getVelocity().x), getVelocity().y});
        m_velChanged_1 = true;
    }
    if (m_life <= 20 && !m_velChanged_2)
    {
        setVelocity({(maxVel + 60) * Helper::sgn(getVelocity().x), getVelocity().y});
        m_velChanged_2 = true;
    }

    if (getPosition().x <= 16.0f)
    {
        setVelocity({-getVelocity().x, getVelocity().y});
    }

    if (getPosition().x + getGlobalBounds().width >= 20.0f * 16.0f)
    {
        setVelocity({-getVelocity().x, getVelocity().y});
    }

    if (m_shootTime >= sf::seconds(3.0f))
    {
        bMan.addBullet(Direction::DOWN, {getPosition().x + m_animation.getSprite().getGlobalBounds().width / 2.0f,
                                         (getPosition().y + m_animation.getSprite().getGlobalBounds().height / 2.0f) + 15.0f}, true, true);
        m_shootTime -= sf::seconds(3.0f);
    }

    m_animation.update(eTime);
}

sf::FloatRect GreenBoss::getGlobalBounds()
{
    sf::FloatRect rect = getTransform().transformRect(m_animation.getSprite().getGlobalBounds());

    rect.left += 5.0f;

    rect.width -= 10.0f;

    return rect;
}

void GreenBoss::respawn()
{
    Boss::respawn();

    m_life = 50;

    setVelocity({maxVel, 0.0f});

    setPosition(50.0f, 35.0f);
}
