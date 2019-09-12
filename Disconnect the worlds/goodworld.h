#ifndef GOODWORLD_H
#define GOODWORLD_H

#include <vector>

#include "world.h"
#include "card.h"

class GoodWorld : public World
{
public:
    GoodWorld(Context& context);

    void update(sf::Time eTime, Game *game);

    void restart(std::array<CardType, 3> goldenCards, Context &context);
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    std::vector<Card::Ptr> m_cards;
};

#endif // GOODWORLD_H
