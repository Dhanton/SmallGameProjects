#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <vector>

#include "enemy.h"

class EnemyManager : public sf::Drawable
{
public:
    EnemyManager(Context& context);

    void update(sf::Time eTime, const ObstacleManager &obsMan,
                const TileMap &map, BulletManager &bMan, ExplosionManager &explMan, Player *player);

    void update(sf::Time eTime, BulletManager &bMan, ExplosionManager &explMan, Player *player);

    void addEnemy(const sf::Vector2f& position);

    void clear();

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    std::vector<Enemy::Ptr> m_enemies;

    const sf::Texture& m_texture;

};

#endif // ENEMYMANAGER_H
