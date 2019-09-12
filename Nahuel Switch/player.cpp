#include "player.h"

#include "tilemap.h"
#include "mathhelper.h"

#include <iostream>

Player::Player(Vector2 position, PlayerInfo info, const std::vector<Ptr> &players):
    m_info(info)
{
    m_players = &players;

    m_gravityKey = info.key;

    respawn(position);

    m_yDir = Helper::randPercentage(0.5f) ? 1 : -1;

    m_sprite.setFillColor(info.color);
    m_sprite.setSize({25.0f, 46.0f});
}

void Player::update(float xScrollingVel, sf::Time eTime)
{
    if (m_dead) return;

    if (!m_parentChunk->getRect().intersects(getRect())) {
        die();
        return;
    }

    m_velocity.y = 500.0f * m_yDir;
    m_velocity.x = xScrollingVel;

    moveColliding(eTime.asSeconds());

    move(m_velocity * eTime.asSeconds());
}

void Player::handleInput(const sf::Event &event)
{
    if (m_dead) return;

    if (event.type == sf::Event::KeyPressed && event.key.code == m_gravityKey) {
        if (m_onFloor) m_yDir = -m_yDir;
    }
}

void Player::setParentChunk(TileMap *tileMap)
{
    if (tileMap == nullptr) return;

    m_parentChunk = tileMap;
}

TileMap *Player::getParentChunk() const
{
    return m_parentChunk;
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::die()
{
    m_dead = true;
}

void Player::respawn(Vector2 position)
{
    setPosition(position);

    m_dead = false;

    m_velocity.y = 100.0f;
}

sf::FloatRect Player::getRect() const
{
    return {getPosition().x, getPosition().y, m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height};
}

PlayerInfo Player::getInfo() const
{
    return m_info;
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (m_dead) return;

    states.transform = getTransform();

    target.draw(m_sprite, states);


#ifdef DEBUG_MODE
    sf::FloatRect drawRect = getRect();

    sf::RectangleShape shape;

    shape.setPosition(drawRect.left, drawRect.top);
    shape.setSize({drawRect.width, drawRect.height});

    shape.setFillColor(sf::Color(0, 0, 255, 100));

    //target.draw(shape);
#endif
}

void Player::moveColliding(float dt)
{
    sf::FloatRect xRect = getRect();
    sf::FloatRect yRect = getRect();

    xRect.left += m_velocity.x * dt;
    yRect.top  += m_velocity.y * dt;

    if (m_parentChunk->isColliding(xRect)) {
        for (float i = 0.0f; i < Helper::abs(m_velocity.x * dt); i += 0.1f) {
            sf::FloatRect resultingRect = getRect();

            resultingRect.left += 0.1f * Helper::sgn(m_velocity.x);

            if (!m_parentChunk->isColliding(resultingRect)) {
                move(0.1f * Helper::sgn(m_velocity.x), 0.0f);
                continue;
            }

            m_velocity.x = 0.0f;
            break;
        }
    }

    if (m_parentChunk->isColliding(yRect)) {
        for (float i = 0.0f; i < Helper::abs(m_velocity.y * dt); i += 0.1f) {
            sf::FloatRect resultingRect = getRect();

            resultingRect.top += 0.1f * Helper::sgn(m_velocity.y);

            if (!m_parentChunk->isColliding(resultingRect)) {
                move(0.0f, 0.1f * Helper::sgn(m_velocity.y));
                m_velocity.y += 0.1f * Helper::sgn(m_velocity.y) / dt;
                continue;
            }

            m_yDir = Helper::sgn(m_velocity.y);
            m_onFloor = true;

            m_velocity.y = 0.0f;
            break;
        }
    }
    else m_onFloor = false;
}

bool Player::isColliding(sf::FloatRect rect) const
{
    for (auto& player : *m_players) {
        if (player.get() == this) continue;

        if (player->getRect().intersects(rect)) return true;
    }

    return false;
}


