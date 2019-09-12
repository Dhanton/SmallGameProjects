#include "player.h"

#include "mathhelper.h"

#include <iostream>

Player::Player(Context &context):
    m_soundPlayer(context.soundPlayer),
    maxVel(100.0f),
    m_shootInterval(sf::milliseconds(200)),
    m_hud(context.textureLoader->getResource(TextureID::OTHERS), {16, 16}),
    m_animation(context.textureLoader->getResource(TextureID::PLAYER), {16, 16})
{    
    m_looking = Direction::RIGHT;

    m_shoot = false;

    m_shootTime = m_shootInterval;

    m_dead = false;
    m_lifes = 5;

    m_isInv = false;

    for (int i = 0; i < m_currentCards.size(); ++i)
        m_currentCards[i] = CardType::NONE;

    for (int i = 0; i < m_goldenCards.size(); ++i)
        m_goldenCards[i] = CardType::NONE;

    /* Character animation stuff */
    m_animation.addAnimation("RIGHT", 6, 1, sf::Time::Zero, true);
    m_animation.addAnimation("LEFT", 8, 1, sf::Time::Zero, true);
    m_animation.addAnimation("DOWN", 0, 1, sf::Time::Zero, true);
    m_animation.addAnimation("UP", 3, 1, sf::Time::Zero, true);

    m_animation.addAnimation("WALKING_RIGHT", 6, 2, sf::seconds(0.3f), true);
    m_animation.addAnimation("WALKING_LEFT", 8, 2, sf::seconds(0.3f), true);
    m_animation.addAnimation("WALKING_DOWN", 0, 3, sf::seconds(0.3f), true);
    m_animation.addAnimation("WALKING_UP", 3, 3, sf::seconds(0.3f), true);

    m_animation.playAnimation("DOWN");

    /* HUD stuff */
    m_hud.addAnimation("FULL_HEART", 30, 1, sf::Time::Zero, true);
    m_hud.addAnimation("EMPTY_HEART", 31, 1, sf::Time::Zero, true);

    m_hud.addAnimation("BLUE_KEY", 32, 1, sf::Time::Zero, true);
    m_hud.addAnimation("RED_KEY", 33, 1, sf::Time::Zero, true);
    m_hud.addAnimation("GREEN_KEY", 42, 1, sf::Time::Zero, true);

    m_hud.addAnimation("GOLDEN_BLUE_KEY", 50, 1, sf::Time::Zero, true);
    m_hud.addAnimation("GOLDEN_RED_KEY", 51, 1, sf::Time::Zero, true);
    m_hud.addAnimation("GOLDEN_GREEN_KEY", 52, 1, sf::Time::Zero, true);

    m_hud.scale(2.0f, 2.0f);
}

void Player::update(sf::Time eTime, const ObstacleManager& obsMan, const TileMap &map, BulletManager &bMan)
{
    m_shootTime += eTime;
    m_invTime += eTime;

    if (m_lifes == 0)
    {
        m_dead = true;
        return;
    }

    if (m_isInv)
    {
        if (m_invTime >= sf::seconds(1.0f))
        {
            m_isInv = false;
        }
    }

    if (m_shootTime >= m_shootInterval)
    {
        if (m_shoot)
        {
            sf::Vector2f pos = getPosition();

            if (m_looking == Direction::DOWN)
            {
                pos.x += m_animation.getSprite().getGlobalBounds().width / 2.0f - 3.0f;
                pos.y += 15.0f;
            }

            if (m_looking == Direction::UP)
            {
                pos.x += m_animation.getSprite().getGlobalBounds().width / 2.0f - 3.0f;
                pos.y -= 8.0f;
            }

            if (m_looking == Direction::RIGHT)
            {
                pos.y += m_animation.getSprite().getGlobalBounds().height / 2.0f;
                pos.x += getGlobalBounds().width + 8.0f;
            }

            if (m_looking == Direction::LEFT)
            {
                pos.y += m_animation.getSprite().getGlobalBounds().height / 2.0f;
            }

            bMan.addBullet(m_looking, pos, true);
            m_shoot = false;

            m_shootTime -= m_shootInterval;
        }

        else m_shootTime = m_shootInterval;
    }

    Entity::update(eTime, obsMan, map);

    updateAnimation(eTime);
}

void Player::update(sf::Time eTime, BulletManager &bMan)
{
    m_shootTime += eTime;
    m_invTime += eTime;

    if (m_lifes == 0)
    {
        m_dead = true;
        return;
    }

    if (m_isInv)
    {
        if (m_invTime >= sf::seconds(1.0f))
        {
            m_isInv = false;
        }
    }

    if (m_shootTime >= m_shootInterval)
    {
        if (m_shoot)
        {
            sf::Vector2f pos = getPosition();

            if (m_looking == Direction::DOWN)
            {
                pos.x += m_animation.getSprite().getGlobalBounds().width / 2.0f - 3.0f;
                pos.y += 15.0f;
            }

            if (m_looking == Direction::UP)
            {
                pos.x += m_animation.getSprite().getGlobalBounds().width / 2.0f - 3.0f;
                pos.y -= 8.0f;
            }

            if (m_looking == Direction::RIGHT)
            {
                pos.y += m_animation.getSprite().getGlobalBounds().height / 2.0f;
                pos.x += getGlobalBounds().width + 8.0f;
            }

            if (m_looking == Direction::LEFT)
            {
                pos.y += m_animation.getSprite().getGlobalBounds().height / 2.0f;
            }

            bMan.addBullet(m_looking, pos, true);
            m_shoot = false;
        }

        m_shootTime = sf::Time::Zero;
    }

    Entity::update(eTime);

    updateAnimation(eTime);
}

void Player::handleInput(const sf::Event &event)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        setVelocity({maxVel, 0.0f});
        m_looking = Direction::RIGHT;
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        setVelocity({-maxVel, 0.0f});
        m_looking = Direction::LEFT;
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        setVelocity({0.0f, -maxVel});
        m_looking = Direction::UP;
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        m_looking = Direction::DOWN;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) setVelocity({0.0f, maxVel});
    }
    else setVelocity({0.0f, 0.0f});

    /* Shoot events */
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Z)
        {
            m_shoot = true;
        }
    }
}

void Player::pickUpCard(CardType type, bool gold)
{
    m_soundPlayer->play(SoundID::PICK_UP, getPosition());

    if (gold)
    {
        for (int i = 0; i < m_goldenCards.size(); ++i)
            if (m_goldenCards[i] == CardType::NONE)
            {
                m_goldenCards[i] = type;
                break;
            }
    }
    else
    {
        for (int i = 0; i < m_currentCards.size(); ++i)
            if (m_currentCards[i] == CardType::NONE)
            {
                m_currentCards[i] = type;
                break;
            }
    }
}

void Player::destroyCard(CardType type)
{
    for (int i = 0; i < m_currentCards.size(); ++i)
    {
        if (m_currentCards[i] == type)
            m_currentCards[i] = CardType::NONE;
    }
}

bool Player::hasCard(CardType type, bool gold) const
{
    if (gold)
    {
        for (int i = 0; i < m_goldenCards.size(); ++i)
            if (m_goldenCards[i] == type) return true;

        return false;
    }
    else
    {
        for (int i = 0; i < m_currentCards.size(); ++i)
            if (m_currentCards[i] == type) return true;

        return false;
    }
}

std::array<CardType, 3> Player::getCurrentCards(bool gold) const
{
    return (gold ? m_goldenCards : m_currentCards);
}

bool Player::canOpenGoldDoor() const
{
    bool can = true;

    for (int i = 0; i < 3; ++i)
        if (m_goldenCards[i] == CardType::NONE) can = false;

    return can;
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::respawn(bool destroyCards)
{
    m_looking = Direction::RIGHT;

    m_shoot = false;

    m_shootTime = m_shootInterval;

    m_dead = false;
    m_lifes = 5;

    if (destroyCards)
    {
        for (int i = 0; i < m_currentCards.size(); ++i)
            m_currentCards[i] = CardType::NONE;
    }
}

void Player::takeDamage()
{
    if (m_lifes == 0 || m_isInv) return;

    m_soundPlayer->play(SoundID::PLAYER_HURT, getPosition());

    m_lifes--;

    m_invTime = sf::Time::Zero;
    m_isInv = true;
}

sf::FloatRect Player::getGlobalBounds()
{
    sf::FloatRect rect = getTransform().transformRect(m_animation.getSprite().getGlobalBounds());

    //if (m_looking == Direction::RIGHT || m_looking == Direction::LEFT)
    //{
        rect.left += 4;
        rect.width -= 9;
    //}

    return rect;
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_animation, states);
}

void Player::updateAnimation(sf::Time eTime)
{
    if (getVelocity().x != 0 || getVelocity().y != 0)
    {
        if (m_looking == Direction::RIGHT) m_animation.playAnimation("WALKING_RIGHT");
        if (m_looking == Direction::LEFT) m_animation.playAnimation("WALKING_LEFT");
        if (m_looking == Direction::DOWN) m_animation.playAnimation("WALKING_DOWN");
        if (m_looking == Direction::UP) m_animation.playAnimation("WALKING_UP");
    }
    else
    {
        if (m_looking == Direction::RIGHT) m_animation.playAnimation("RIGHT");
        if (m_looking == Direction::LEFT) m_animation.playAnimation("LEFT");
        if (m_looking == Direction::DOWN) m_animation.playAnimation("DOWN");
        if (m_looking == Direction::UP) m_animation.playAnimation("UP");
    }

    m_animation.update(eTime);
}

void Player::drawHud(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::View previousView = target.getView();
    sf::View view = target.getDefaultView();

    target.setView(view);

    m_hud.getSprite().setScale(1.0f, 1.0f);

    /* Lifes */
    int maxLifes = 5;
    int emptyHearts = maxLifes - m_lifes;

    for (int i = 0; i < m_lifes; ++i)
    {
        m_hud.playAnimation("FULL_HEART");
        m_hud.setPosition(20 * i, 10.0f);
        target.draw(m_hud, states);
    }

    for (int i = 0; i < emptyHearts; ++i)
    {
        m_hud.playAnimation("EMPTY_HEART");
        m_hud.setPosition(20 * i + (m_lifes) * 20, 10.0f);
        target.draw(m_hud, states);
    }

    m_hud.getSprite().setScale(2.0f, 2.0f);

    /* Blue card */
    if (hasCard(CardType::BLUE, true))
    {
        m_hud.playAnimation("GOLDEN_BLUE_KEY");
        m_hud.setPosition(10.0f, 47.0f);
        target.draw(m_hud, states);
    }
    else if (hasCard(CardType::BLUE))
    {
        m_hud.playAnimation("BLUE_KEY");
        m_hud.setPosition(10.0f, 47.0f);
        target.draw(m_hud, states);
    }

    /* Green Card */
    if (hasCard(CardType::GREEN, true))
    {
        m_hud.playAnimation("GOLDEN_GREEN_KEY");
        m_hud.setPosition(30.0f, 47.0f);
        target.draw(m_hud, states);
    }
    else if (hasCard(CardType::GREEN))
    {
        m_hud.playAnimation("GREEN_KEY");
        m_hud.setPosition(30.0f, 47.0f);
        target.draw(m_hud, states);
    }

    /* Red Card */
    if (hasCard(CardType::RED, true))
    {
        m_hud.playAnimation("GOLDEN_RED_KEY");
        m_hud.setPosition(50.0f, 47.0f);
        target.draw(m_hud, states);
    }
    else if (hasCard(CardType::RED))
    {
        m_hud.playAnimation("RED_KEY");
        m_hud.setPosition(50.0f, 47.0f);
        target.draw(m_hud, states);
    }

    target.setView(previousView);
}

