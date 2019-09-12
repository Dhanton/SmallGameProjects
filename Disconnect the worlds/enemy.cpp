#include "enemy.h"

#include "vector2.h"
#include "mathhelper.h"

Enemy::Enemy(const sf::Texture &texture):
    m_animation(texture, {16, 16}),
    maxVel(100.0f),
    minDist(70.0f)
{
    m_animation.addAnimation("MAIN", 0, 2, sf::seconds(2.0f), true);
    m_animation.getSprite().setScale(2.0f, 2.0f);

    m_animation.playAnimation("MAIN");

    m_lifes = 1;

    setRandomDirection();
    computeDirection();

    m_isFollowing = false;
}

void Enemy::update(sf::Time eTime, const ObstacleManager &obsMan, const TileMap &map,
                   BulletManager &bMan, ExplosionManager &explMan, Player* player)
{
    if (m_lifes == 0)
    {
        m_dead = true;
        explMan.addExplosion({getPosition().x - 6.0f, getPosition().y - 6.0f});
        return;
    }

    m_toNewDirection += eTime;

    if (player->getGlobalBounds().intersects(getGlobalBounds()))
    {
        m_lifes = 0;
        player->takeDamage();
        return;
    }

    if (m_isFollowing)
    {
        followPlayer(player);

        if (getDistance(player->getPosition()) > minDist)
        {
            m_isFollowing = false;
            m_toNewDirection = sf::Time::Zero;
        }
    }
    else
    {
        if (m_toNewDirection >= sf::seconds(2.0f) || (getVelocity().x == 0.0f && getVelocity().y == 0.0f))
        {
            m_toNewDirection -= sf::seconds(2.0f);
            setRandomDirection();
            computeDirection();
        }

        if (getDistance(player->getPosition()) <= minDist)
        {
            m_isFollowing = true;
        }
    }

    Entity::update(eTime, obsMan, map);

    if (bMan.checkCollision(getGlobalBounds(), true))
    {
        takeDamage();
        return;
    }

    m_animation.update(eTime);
}

void Enemy::update(sf::Time eTime, BulletManager& bMan, ExplosionManager& explMan, Player* player)
{
    if (m_lifes == 0)
    {
        m_dead = true;
        explMan.addExplosion({getPosition().x - 6.0f, getPosition().y - 6.0f});
        return;
    }

    m_toNewDirection += eTime;

    if (player->getGlobalBounds().intersects(getGlobalBounds()))
    {
        m_lifes = 0;
        player->takeDamage();
        return;
    }

    followPlayer(player);

    if (getDistance(player->getPosition()) > minDist)
    {
        m_isFollowing = false;
        m_toNewDirection = sf::Time::Zero;
    }

    Entity::update(eTime);

    if (bMan.checkCollision(getGlobalBounds(), true))
    {
        takeDamage();
        return;
    }

    m_animation.update(eTime);
}

void Enemy::takeDamage()
{
    if (m_lifes == 0) return;

    m_lifes--;
}

sf::FloatRect Enemy::getGlobalBounds()
{
    sf::FloatRect rect = getTransform().transformRect(m_animation.getSprite().getGlobalBounds());

    rect.width = 16.0f;
    rect.height = 18.0f;

    return rect;
}

bool Enemy::isDead() const
{
    return m_dead;
}

void Enemy::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_animation, states);
}

void Enemy::setRandomDirection()
{
    int dir = Helper::randRange(0, 4);

    if (dir == static_cast<int>(m_dir))
        setRandomDirection();

    m_dir = static_cast<Direction>(dir);
}

void Enemy::computeDirection()
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

void Enemy::followPlayer(Player *player)
{
    Vector2 position = player->getPosition();

    Vector2 direction = position - getPosition();

    setVelocity(Helper::normalize(direction) * maxVel);
}

float Enemy::getDistance(const sf::Vector2f &point)
{
    Vector2 direction = point - getPosition();

    return Helper::length(direction);
}
