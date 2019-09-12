#include "card.h"

Card::Card(CardType type, Context& context, const sf::Vector2f& pos):
    m_animation(context.textureLoader->getResource(TextureID::OTHERS), {16, 16})
{
    m_animation.addAnimation("BLUE", 32, 1, sf::Time::Zero, true);
    m_animation.addAnimation("RED", 33, 1, sf::Time::Zero, true);
    m_animation.addAnimation("GREEN", 42, 1, sf::Time::Zero, true);

    setPosition(pos);

    m_animation.getSprite().setScale(2.0f, 2.0f);

    switch (type)
    {
    case CardType::BLUE:
        m_animation.playAnimation("BLUE");
        break;
    case CardType::GREEN:
        m_animation.playAnimation("GREEN");
        break;
    case CardType::RED:
        m_animation.playAnimation("RED");
        break;
    }

    m_type = type;
    m_destroyed = false;
}

void Card::update(sf::Time eTime)
{
    m_animation.update(eTime);
}

bool Card::isDestroyed() const
{
    return m_destroyed;
}

void Card::destroy()
{
    m_destroyed = true;
}

sf::FloatRect Card::getGlobalBounds()
{
    sf::FloatRect rect = getTransform().transformRect(m_animation.getSprite().getGlobalBounds());

    rect.width = 6.0f;
    rect.height = 12.0f;

    return rect;
}

CardType Card::getType() const
{
    return m_type;
}

void Card::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_animation, states);
}
