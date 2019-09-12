#include "portal.h"

Portal::Portal(const sf::Texture& texture):
    m_animation(texture, {16, 16})
{
    m_animation.addAnimation("MAIN", 41, 5, sf::milliseconds(400.0f), true);

    m_animation.playAnimation("MAIN");
}

void Portal::update(sf::Time eTime)
{
    m_animation.update(eTime);
}

bool Portal::isColliding(const sf::FloatRect &rect)
{
    sf::FloatRect animRect = getTransform().transformRect(m_animation.getSprite().getGlobalBounds());

    return rect.intersects(animRect);
}

void Portal::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_animation, states);
}
