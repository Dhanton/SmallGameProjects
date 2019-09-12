#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <map>
#include <stdexcept>
#include <cassert>

#include "animation.h"

class AnimationManager : public sf::Drawable, public sf::Transformable
{
public:
    AnimationManager(const sf::Texture& texture, const sf::Vector2i size);

    void update(sf::Time eTime);

    void addAnimation(const std::string& name, unsigned int initialFrame, unsigned int frameNumber,
                      sf::Time duration, bool repeat);

    void playAnimation(const std::string& name);

    Animation* getCurrentAnimation() const;

    sf::Sprite& getSprite();

    sf::FloatRect getSpriteBounds() const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    std::map<std::string, Animation> m_animationMap;
    Animation*                       m_currentAnimation;

    sf::Sprite m_sprite;

    const sf::Vector2i m_size;
};

#endif // ANIMATIONMANAGER_H

