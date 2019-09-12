#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "bullet.h"

class BulletManager : public sf::Drawable
{
public:
    BulletManager(Context& context, TextureID id);

    void update(sf::Time eTime, const ObstacleManager &obsMan);
    void update(sf::Time eTime);

    void addBullet(Direction dir, const sf::Vector2f& pos, bool playSound, bool enemy = false);

    bool checkCollision(sf::FloatRect rect, bool shouldDestroy = false, bool enemy = false);

    void clear();

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    SoundPlayer<>* m_soundPlayer;

    std::vector<Bullet::Ptr> m_bullets;

    const sf::Texture& m_texture;
};

#endif // BULLETMANAGER_H
