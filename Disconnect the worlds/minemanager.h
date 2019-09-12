#ifndef MINEMANAGER_H
#define MINEMANAGER_H

#include <list>

#include "animationmanager.h"
#include "explosionmanager.h"
#include "context.h"

class MineManager : public sf::Drawable
{
public:
    MineManager(Context& context);

    void update(sf::Time eTime, ExplosionManager &explosionMan);

    void addMine(const sf::Vector2f position);

    bool checkCollision(sf::FloatRect rect, ExplosionManager& explosionMan);

    void clear();

    bool isEmtpy() const;
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    std::list<std::unique_ptr<AnimationManager>> m_mines;

    const sf::Texture& m_texture;
};

#endif // MINEMANAGER_H
