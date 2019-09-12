#ifndef CARD_H
#define CARD_H

#include <SFML/Graphics.hpp>

#include "animationmanager.h"
#include "context.h"

enum class CardType
{
    NONE,
    BLUE,
    RED,
    GREEN
};

class Card : public sf::Drawable, public sf::Transformable
{
public:
    typedef std::unique_ptr<Card> Ptr;

public:
    Card(CardType type, Context &context, const sf::Vector2f &pos);

    void update(sf::Time eTime);

    bool isDestroyed() const;
    void destroy();

    sf::FloatRect getGlobalBounds();
    CardType getType() const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    AnimationManager m_animation;

    CardType m_type;

    bool m_destroyed;
};

#endif // CARD_H
