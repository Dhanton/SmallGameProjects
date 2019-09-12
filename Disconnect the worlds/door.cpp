#include "door.h"

Door::Door(CardType key, Context& context, const sf::Vector2f &pos, bool gold):
    m_animation(context.textureLoader->getResource(TextureID::OTHERS), {16, 16})
{
    setPosition(pos);

    m_animation.addAnimation("RED", 14, 1, sf::Time::Zero, true);
    m_animation.addAnimation("BLUE", 15, 1, sf::Time::Zero, true);
    m_animation.addAnimation("GREEN", 16, 1, sf::Time::Zero, true);
    m_animation.addAnimation("GOLD", 17, 1, sf::Time::Zero, true);

    m_animation.getSprite().scale(2.0f, 2.0f);

    switch (key)
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

    if (gold) m_animation.playAnimation("GOLD");

    m_gold = gold;
    m_key = key;
}

void Door::update(sf::Time eTime)
{
    m_animation.update(eTime);
}

CardType Door::getKeyType()
{
    return m_key;
}

bool Door::isGold() const
{
    return m_gold;
}

bool Door::tryOpen(std::array<CardType, 3> key)
{
    for (int i = 0; i < key.size(); ++i)
        if (key[i] == m_key) return true;

    return false;
}

sf::FloatRect Door::getGlobalBounds()
{
    sf::FloatRect rect = getTransform().transformRect(m_animation.getSprite().getGlobalBounds());

    rect.width = 16.0f;
    rect.height = 26.0f;

    return rect;
}

void Door::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_animation, states);
}
