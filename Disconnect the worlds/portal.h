#ifndef PORTAL_H
#define PORTAL_H

#include <SFML/Graphics.hpp>

#include "animationmanager.h"

class Portal : public sf::Drawable, public sf::Transformable
{
public:
    Portal(const sf::Texture &texture);

    void update(sf::Time eTime);

    bool isColliding(const sf::FloatRect& rect);

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    AnimationManager m_animation;
};

#endif // PORTAL_H
