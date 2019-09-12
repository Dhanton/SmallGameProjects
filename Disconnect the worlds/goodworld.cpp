#include "goodworld.h"

GoodWorld::GoodWorld(Context &context):
    World(context, TextureID::GOOD_WORLD)
{
    m_type = Type::GOOD;

    m_tileMap.loadFromImage("levels/world.png");

    m_obs.loadFromFile("levels/other_data.png", false);

    m_cards.push_back(std::move(Card::Ptr(new Card(CardType::BLUE, context, {30.0f, 30.0f}))));
    m_cards.push_back(std::move(Card::Ptr(new Card(CardType::GREEN, context, {43.0f * 16.0f, 40.0f}))));
    m_cards.push_back(std::move(Card::Ptr(new Card(CardType::RED, context, {30.0f, 43.0f * 16.0f})))); }

void GoodWorld::update(sf::Time eTime, Game *game)
{
    World::update(eTime, game);

    for (auto& card : m_cards)
    {
        if (card->getGlobalBounds().intersects(m_player->getGlobalBounds()))
        {
            m_player->pickUpCard(card->getType());
            card->destroy();
        }else{
            card->update(eTime);
        }
    }

    m_cards.erase(std::remove_if(m_cards.begin(), m_cards.end(),
                                 [&] (Card::Ptr const &c){return c->isDestroyed();}),
            m_cards.end());
}

void GoodWorld::restart(std::array<CardType, 3> goldenCards, Context& context)
{
    m_cards.clear();

    bool blue = true, red = true, green = true;

    for (int i = 0; i < 3; ++i)
    {
        if (goldenCards[i] == CardType::BLUE) blue = false;
        if (goldenCards[i] == CardType::GREEN) green = false;
        if (goldenCards[i] == CardType::RED) red = false;
    }

    if (blue)  m_cards.push_back(std::move(Card::Ptr(new Card(CardType::BLUE, context, {30.0f, 30.0f}))));
    if (green) m_cards.push_back(std::move(Card::Ptr(new Card(CardType::GREEN, context, {43.0f * 16.0f, 40.0f}))));
    if (red)   m_cards.push_back(std::move(Card::Ptr(new Card(CardType::RED, context, {30.0f, 47.0f * 16.0f}))));
}

void GoodWorld::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_tileMap, states);

    target.draw(m_portal, states);

    target.draw(*m_player, states);

    target.draw(m_bulletMan, states);

    for (auto& card : m_cards)
    {
        target.draw(*card, states);
    }

    target.draw(m_obs, states);

    m_player->drawHud(target, states);
}

