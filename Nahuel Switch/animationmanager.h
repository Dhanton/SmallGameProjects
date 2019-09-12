#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <map>
#include <stdexcept>
#include <cassert>

#include "animation.h"

class AnimationManager : public sf::Drawable, public sf::Transformable
{
public:
    AnimationManager(const sf::Texture& texture, const sf::Vector2<size_t> size);

    void update(sf::Time eTime);

    void addFrame(const std::string& name, size_t initialFrame, size_t frameNumber,
                  sf::Time duration, bool repeat);

    void playAnimation(const std::string& name);

    sf::Sprite& getSprite();

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    std::map<std::string, Animation> m_animationMap;
    Animation*                       m_currentAnimation;

    sf::Sprite m_sprite;

    const sf::Vector2<size_t> m_size;
};

#endif // ANIMATIONMANAGER_H

