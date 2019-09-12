#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "vector2.h"

class TileMap;

struct PlayerInfo
{
    PlayerInfo(sf::Color c, sf::Keyboard::Key k)
    {
        color = c;
        key = k;
    }

    sf::Color         color;
    sf::Keyboard::Key key;
};

inline bool operator==(const PlayerInfo& a, const PlayerInfo& b)
{
    return a.key == b.key;
}

struct PlayerScore
{
    PlayerScore(PlayerInfo i, float s):
        info(i.color, i.key)
    {
        score = s;
    }

    PlayerInfo info;
    float     score;
};

inline bool operator<(const PlayerScore& a, const PlayerScore& b)
{
    return a.score > b.score;
}

class Player : public sf::Drawable, public sf::Transformable
{
public:
    using Ptr = std::unique_ptr<Player>;

public:
    Player(Vector2 position, PlayerInfo info, const std::vector<Player::Ptr>& players);

    void update(float xScrollingVel, sf::Time eTime);

    void handleInput(const sf::Event& event);

    void     setParentChunk(TileMap* tileMap);
    TileMap* getParentChunk() const;

    bool isDead() const;
    void die();
    void respawn(Vector2 position);

    sf::FloatRect getRect() const;

    PlayerInfo getInfo() const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void moveColliding(float dt);

    bool isColliding(sf::FloatRect rect) const;

private:
    TileMap* m_parentChunk;
    const std::vector<Player::Ptr>* m_players;

    Vector2 m_velocity;

    bool m_dead;

    const PlayerInfo m_info;

    bool m_onFloor;
    int  m_yDir;

    sf::Keyboard::Key m_gravityKey;

    sf::RectangleShape m_sprite;
};

#endif // PLAYER_H
