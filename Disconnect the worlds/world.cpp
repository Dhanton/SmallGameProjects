#include "world.h"

#include "game.h"

World::World(Context &context, TextureID id):
    m_soundPlayer(context.soundPlayer),
    m_tileMap(context, id),
    m_obs(context, id),
    m_bulletMan(context, id),
    m_portal(context.textureLoader->getResource(id))
{
    m_player = nullptr;

    m_portal.setPosition(430.0f, 430.0f);

    m_shouldChangeWorlds = false;
}

void World::update(sf::Time eTime, Game* game)
{
    m_portal.update(eTime);

    m_bulletMan.update(eTime, m_obs);

    m_player->update(eTime, m_obs, m_tileMap, m_bulletMan);

    if (m_portal.isColliding(m_player->getGlobalBounds()))
    {
        if (m_shouldChangeWorlds){
            game->changeWorld();
            m_shouldChangeWorlds = false;

            m_soundPlayer->play(SoundID::PORTAL, m_portal.getPosition());
        }
    }
}

void World::handleInput(const sf::Event &event)
{
    m_player->handleInput(event);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
        m_shouldChangeWorlds = true;

    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::X)
        m_shouldChangeWorlds = false;
}

void World::setPlayer(Player *player)
{
    m_player = player;
}

void World::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_tileMap, states);

    target.draw(m_portal, states);

    target.draw(m_bulletMan, states);

    target.draw(*m_player, states);

    target.draw(m_obs, states);

    m_player->drawHud(target, states);
}
