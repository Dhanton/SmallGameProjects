#ifndef DOOR_H
#define DOOR_H

#include <SFML/Graphics.hpp>

#include "animationmanager.h"
#include "card.h"
#include "player.h"

class Door : public sf::Drawable, public sf::Transformable
{
public:
    typedef std::unique_ptr<Door> Ptr;

public:
    Door(CardType key, Context &context, const sf::Vector2f& pos, bool gold = false);

    void update(sf::Time eTime);

    CardType getKeyType();

    bool isGold() const;

    bool tryOpen(std::array<CardType, 3> key);

    sf::FloatRect getGlobalBounds();

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    AnimationManager m_animation;

    CardType m_key;

    bool m_gold;
};

#endif // DOOR_H
