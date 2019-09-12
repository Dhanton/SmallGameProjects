#include "enemymanager.h"

EnemyManager::EnemyManager(Context &context):
    m_texture(context.textureLoader->getResource(TextureID::OTHERS))
{
}

void EnemyManager::update(sf::Time eTime, const ObstacleManager &obsMan, const TileMap& map,
                          BulletManager &bMan, ExplosionManager& explMan, Player* player)
{
    for (auto& enemy : m_enemies)
    {
        enemy->update(eTime, obsMan, map, bMan, explMan, player);
    }

    m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
                                   [&] (Enemy::Ptr const &e){return e->isDead();}),
            m_enemies.end());
}

void EnemyManager::update(sf::Time eTime, BulletManager &bMan, ExplosionManager &explMan, Player *player)
{
    for (auto& enemy : m_enemies)
    {
        enemy->update(eTime, bMan, explMan, player);
    }

    m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
                                   [&] (Enemy::Ptr const &e){return e->isDead();}),
            m_enemies.end());
}

void EnemyManager::addEnemy(const sf::Vector2f &position)
{
    Enemy::Ptr enemy{new Enemy(m_texture)};

    enemy->setPosition(position);

    m_enemies.push_back(std::move(enemy));
}

void EnemyManager::clear()
{
    m_enemies.clear();
}

void EnemyManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto& enemy : m_enemies)
    {
        target.draw(*enemy, states);
    }
}
