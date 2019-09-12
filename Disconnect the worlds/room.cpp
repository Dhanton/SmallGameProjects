#include "room.h"

#include "game.h"
#include "mathhelper.h"

Room::Room(Context &context, Player *player, CardType type, bool isFinal):
    TileSet(context, TextureID::OTHERS),
    m_bulletMan(context, TextureID::BAD_WORLD),
    m_explosionMan(context),
    m_enemyMan(context),
    m_mineMan(context)
{
    m_player = player;

    m_player->setPosition(300.0f, 200.0f);

    m_type = type;

    switch (type)
    {
    case CardType::BLUE:
        m_boss = Boss::Ptr(new BlueBoss(context, TextureID::OTHERS));
        break;
    case CardType::GREEN:
        m_boss = Boss::Ptr(new GreenBoss(context, TextureID::OTHERS));
        break;
    case CardType::RED:
        m_boss = Boss::Ptr(new RedBoss(context, TextureID::OTHERS));
        break;
    default:
        break;
    }

    m_isFinal = isFinal;

    if (isFinal)
    {
        m_boss = Boss::Ptr(new FinalBoss(context, TextureID::OTHERS));
    }

    m_exit = false;
    m_endExplosions = false;

    m_explosionCount = 0;
}

void Room::update(sf::Time eTime, Game *game)
{
    if (m_player->isDead())
    {
        m_restartTime += eTime;

        if (m_restartTime >= sf::milliseconds(500.0f))
        {
            m_restartTime = sf::Time::Zero;
            restart();
        }
        return;
    }

    if (m_boss->isDead())
    {
        if (m_explosionMan.isEmpty() && m_endExplosions)
        {
            if (m_isFinal)
            {
                game->finishGame();
                return;
            }

            game->finishBossBattle();
            m_player->pickUpCard(m_type, true);
            m_player->destroyCard(m_type);
            m_player->respawn(false);
            return;
        }

        if (!m_endExplosions)
        {
            m_bulletMan.update(eTime);
            updateEndExplosions(eTime);
        }
        m_explosionMan.update(eTime);
        return;
    }

    m_bulletMan.update(eTime);

    if (m_bulletMan.checkCollision(m_boss->getGlobalBounds(), true))
    {
        m_boss->takeDamage();
    }

    if (m_bulletMan.checkCollision(m_player->getGlobalBounds(), true, true))
    {
        m_player->takeDamage();
    }

    m_mineMan.update(eTime, m_explosionMan);

    if (m_mineMan.checkCollision(m_player->getGlobalBounds(), m_explosionMan))
    {
        m_player->takeDamage();
    }

    m_boss->update(eTime, m_enemyMan, m_bulletMan, m_mineMan);

    m_player->update(eTime, m_bulletMan);

    m_enemyMan.update(eTime, m_bulletMan, m_explosionMan, m_player);

    if (m_player->getPosition().x  + m_player->getGlobalBounds().width >= 21.0f * 16.0f - 10.0f)
        m_player->setPosition(21.0f * 16.0f - m_player->getGlobalBounds().width - 10.0f, m_player->getPosition().y);

    if (m_player->getPosition().x <= 16.0f) m_player->setPosition(16.0f, m_player->getPosition().y);

    if (m_player->getPosition().y  + m_player->getGlobalBounds().height >= 14.0f * 16.0f)
        m_player->setPosition(m_player->getPosition().x, 14.0f * 16.0f - m_player->getGlobalBounds().height);

    if (m_player->getPosition().y <= 25.0f) m_player->setPosition(m_player->getPosition().x, 25.0f);

    if (m_boss->getGlobalBounds().intersects(m_player->getGlobalBounds()))
    {
        m_player->takeDamage();
    }

    m_explosionMan.update(eTime);
}

void Room::handleInput(const sf::Event &event)
{
    m_player->handleInput(event);
}

void Room::addTile(const sf::Vector2i pos, Room::TileType type)
{
    const unsigned int size = 16;

    sf::Vector2f worldPos = {(float) pos.x * size, (float) pos.y * size};

    sf::Vector2u textPos = toTexturePos(type);

    addVertex(worldPos.x, worldPos.y, textPos.x, textPos.y);
    addVertex(worldPos.x + size, worldPos.y, textPos.x + size, textPos.y);
    addVertex(worldPos.x + size, worldPos.y + size, textPos.x + size, textPos.y + size);
    addVertex(worldPos.x, worldPos.y + size, textPos.x, textPos.y + size);
}

void Room::updateEndExplosions(sf::Time eTime)
{
    m_explosionElapsed += eTime;

    if (m_explosionElapsed >= sf::seconds(0.1f) && !m_endExplosions)
    {
        m_explosionElapsed -= sf::seconds(0.1f);

        sf::Vector2f vec;

        vec.x = Helper::randRange(m_boss->getGlobalBounds().left - 20.0f, m_boss->getGlobalBounds().left + m_boss->getGlobalBounds().width);
        vec.y = Helper::randRange(m_boss->getGlobalBounds().top - 20.0f, m_boss->getGlobalBounds().top + m_boss->getGlobalBounds().height);

        m_explosionMan.addExplosion(vec);

        m_explosionCount++;
    }

    if (m_explosionCount >= 15) m_endExplosions = true;
}

sf::Vector2u Room::toTexturePos(Room::TileType type)
{
    switch (type)
    {
    case TileType::WALL_1:
        return {4 * 16, 0};
    case TileType::WALL_2:
        return {5 * 16, 0};
    case TileType::FLOOR:
        return {6 * 16, 0};
    }
}

void Room::loadRoom()
{
    unsigned int size_x = 22;
    unsigned int size_y = 15;

    for (unsigned int i = 0; i < size_x; ++i)
        for (unsigned int j = 0; j < size_y; ++j)
        {
            if (i == 0 || i == size_x - 1 || j == 0 || j == size_y - 1)
                addTile({i, j}, TileType::WALL_1);

            else if (j == 1)
                addTile({i, j}, TileType::WALL_2);

            else addTile({i, j}, TileType::FLOOR);
        }
}

void Room::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.texture = &m_texture;

    target.draw(m_tileset, states);

    target.draw(m_enemyMan, states);

    target.draw(m_mineMan, states);

    target.draw(*m_player, states);
    target.draw(*m_boss, states);

    target.draw(m_bulletMan, states);

    target.draw(m_explosionMan, states);

    m_player->drawHud(target, states);
}

void Room::restart()
{
    m_boss->setPosition(100.0f, 100.0f);
    m_boss->respawn();

    m_player->respawn(false);
    m_player->setPosition(300.0f, 200.0f);

    m_bulletMan.clear();
    m_explosionMan.clear();
    m_enemyMan.clear();
    m_mineMan.clear();
}
