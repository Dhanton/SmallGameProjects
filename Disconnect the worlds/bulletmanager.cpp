#include "bulletmanager.h"

BulletManager::BulletManager(Context &context, TextureID id):
    m_soundPlayer(context.soundPlayer),
    m_texture(context.textureLoader->getResource(id))
{
}

void BulletManager::update(sf::Time eTime)
{
    for (auto& bullet : m_bullets)
    {
        bullet->update(eTime);

        if (bullet->getPosition().x >= 21.0f * 16.0f) bullet->destroy();
        if (bullet->getPosition().y >= 14.0f * 16.0f) bullet->destroy();

        if (bullet->getPosition().x <= 16.0f) bullet->destroy();
        if (bullet->getPosition().y <= 24.0f) bullet->destroy();
    }

    m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
                    [&] (Bullet::Ptr const &b){return b->isDead();}), m_bullets.end());
}

void BulletManager::update(sf::Time eTime, const ObstacleManager &obsMan)
{
    for (auto& bullet : m_bullets)
    {
        bullet->update(eTime, obsMan);

        if (bullet->getPosition().x <= 0.0f) bullet->destroy();
        if (bullet->getPosition().y <= 0.0f) bullet->destroy();
    }

    m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(),
                    [&] (Bullet::Ptr const &b){return b->isDead();}), m_bullets.end());
}

void BulletManager::addBullet(Direction dir, const sf::Vector2f &pos, bool playSound, bool enemy)
{
    Bullet::Ptr bullet{new Bullet(dir, m_texture, enemy)};

    bullet->setPosition(pos);

    if (playSound) m_soundPlayer->play(SoundID::BULLET, pos);

    m_bullets.push_back(std::move(bullet));
}

bool BulletManager::checkCollision(sf::FloatRect rect, bool shouldDestroy, bool enemy)
{
    for (auto& bullet : m_bullets)
    {
        if (bullet->getGlobalBounds().intersects(rect))
        {
            if (enemy)
            {
                if (bullet->isEnemy())
                {
                    if (shouldDestroy) bullet->destroy();
                    return true;
                }
            }
            else
            {
                if (!bullet->isEnemy())
                {
                    if (shouldDestroy) bullet->destroy();
                    return true;
                }
            }
        }
    }

    return false;
}

void BulletManager::clear()
{
    m_bullets.clear();
}

void BulletManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto& bullet : m_bullets)
    {
        target.draw(*bullet, states);
    }
}
