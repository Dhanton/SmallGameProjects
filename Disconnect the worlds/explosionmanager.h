#ifndef EXPLOSIONMANAGER_H
#define EXPLOSIONMANAGER_H

#include <vector>
#include <memory>

#include "context.h"
#include "animationmanager.h"

class ExplosionManager : public sf::Drawable
{
public:
    ExplosionManager(Context &context);

    void update(sf::Time eTime);

    void addExplosion(const sf::Vector2f& position);

    void clear();

    bool isEmpty() const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    SoundPlayer<>* m_soundPlayer;

    std::vector<std::unique_ptr<AnimationManager>> m_explosions;

    const sf::Texture& m_texture;

    const sf::Time m_duration;
};

#endif // EXPLOSIONMANAGER_H
