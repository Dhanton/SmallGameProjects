#include "hell.h"

#include "game.h"
#include <iostream>

Hell::Hell(Context &context):
    World(context, TextureID::BAD_WORLD),
    m_soundPlayer(context.soundPlayer),
    m_enemyMan(context),
    m_explosionMan(context)
{
   m_type = Type::HELL;

   m_tileMap.loadFromImage("levels/world.png");

   m_obs.loadFromFile("levels/other_data.png", true);

   respawnEnemies();

   m_doors.push_back(std::move(Door::Ptr(new Door(CardType::BLUE, context, {30.0f, 30.0f}))));
   m_doors.push_back(std::move(Door::Ptr(new Door(CardType::GREEN, context, {45.0f * 16.0f, 40.0f}))));
   m_doors.push_back(std::move(Door::Ptr(new Door(CardType::RED, context, {30.0f, 47.0f * 16.0f}))));
   m_doors.push_back(std::move(Door::Ptr(new Door(CardType::NONE, context, {430.0f, 350.0f}, true))));
}

void Hell::update(sf::Time eTime, Game *game)
{
    World::update(eTime, game);

    m_enemyMan.update(eTime, m_obs, m_tileMap, m_bulletMan, m_explosionMan, m_player);

    for (auto& door : m_doors)
    {
        door->update(eTime);

        if (door->getGlobalBounds().intersects(m_player->getGlobalBounds()))
        {
            if (door->isGold())
            {
                if (m_player->canOpenGoldDoor())
                {
                    game->startBossBattle(CardType::NONE, true);
                    m_soundPlayer->play(SoundID::DOOR, door->getPosition());
                    return;
                }
            }

            else if (door->tryOpen(m_player->getCurrentCards()))
            {
                game->startBossBattle(door->getKeyType());
                m_soundPlayer->play(SoundID::DOOR, door->getPosition());
            }
        }
    }

    m_explosionMan.update(eTime);
}

void Hell::restart()
{
    m_enemyMan.clear();
    m_explosionMan.clear();
    m_bulletMan.clear();

    respawnEnemies();
}

void Hell::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_tileMap, states);

    for (auto& door : m_doors)
    {
        target.draw(*door, states);
    }

    target.draw(m_portal, states);

    target.draw(*m_player, states);

    target.draw(m_bulletMan, states);

    target.draw(m_enemyMan, states);

    target.draw(m_explosionMan, states);

    target.draw(m_obs, states);

    m_player->drawHud(target, states);
}

void Hell::respawnEnemies()
{
    sf::Image image;

    if (!image.loadFromFile("levels/other_data.png")) return;

    for (unsigned int i = 0; i < image.getSize().x; ++i)
        for (unsigned int j = 0; j < image.getSize().y; ++j)
        {
            sf::Color c = image.getPixel(i, j);

            if (c == sf::Color::White) m_enemyMan.addEnemy({static_cast<float>(i * 16.0f), static_cast<float>(j * 16.0f)});
        }
}
