#include "game.h"

Game::Game(Context &context):
    m_world(context),
    m_hell(context),
    m_context(context)
{
    m_context = context;

    m_player = Player::Ptr(new Player(context));
    m_player->setPosition(300.0f, 300.0f);

    m_world.setPlayer(m_player.get());
    m_hell.setPlayer(m_player.get());

    m_isHell = false;
    m_needsChange = false;

    m_finished = false;

    m_currentRoom = nullptr;

    //startBossBattle(CardType::BLUE);
}

void Game::update(sf::Time eTime)
{
    if (m_currentRoom == nullptr)
    {
        if (m_player->isDead())
        {
            m_player->respawn();
            m_player->setPosition(300.0f, 300.0f);
            m_world.restart(m_player->getCurrentCards(true), m_context);
            m_hell.restart();
            m_isHell = false;
        }

        if (!m_isHell)
            m_world.update(eTime, this);
        else
            m_hell.update(eTime, this);
    }
    else
    {
        m_currentRoom->update(eTime, this);
    }

    m_context.soundPlayer->setListenerPosition(m_player->getPosition());
}

void Game::handleInput(const sf::Event &event)
{
    if (m_currentRoom == nullptr)
    {
        if (!m_isHell)
            m_world.handleInput(event);
        else
            m_hell.handleInput(event);
    }
    else
    {
        m_currentRoom->handleInput(event);
    }
}

bool Game::isFinished() const
{
    return m_finished;
}

void Game::finishGame()
{
    m_finished = true;
}

void Game::changeWorld()
{
    m_isHell = m_isHell ? false : true;
}

void Game::startBossBattle(CardType type, bool finalBoss)
{
    m_currentRoom = Room::Ptr(new Room(m_context, m_player.get(), type, finalBoss));
    m_currentRoom->loadRoom();
}

void Game::finishBossBattle()
{
    m_currentRoom = nullptr;

    m_isHell = false;

    m_player->setPosition(300.0f, 300.0f);
}

sf::Vector2f Game::getPlayerPos() const
{
    return m_player->getPosition();
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (m_currentRoom == nullptr)
    {
        if (!m_isHell)
            target.draw(m_world, states);
        else
            target.draw(m_hell, states);
    }
    else
    {
        sf::View view = target.getDefaultView();
        view.zoom(1.0f/3.0f);
        view.setCenter(view.getSize() / 2.0f);
        view.setCenter(view.getCenter().x - 5.0f, view.getCenter().y);

        target.setView(view);

        target.draw(*m_currentRoom, states);
    }
}
