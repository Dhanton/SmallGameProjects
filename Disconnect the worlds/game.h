#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "player.h"
#include "goodworld.h"
#include "hell.h"
#include "room.h"

class Game : public sf::Drawable
{
public:
    typedef std::unique_ptr<Game> Ptr;

public:
    Game(Context& context);

    void update(sf::Time eTime);

    void handleInput(const sf::Event& event);

    bool isFinished() const;

    void finishGame();

    void printText(const std::string& text);

    void changeWorld();

    void startBossBattle(CardType type, bool finalBoss = false);
    void finishBossBattle();

    sf::Vector2f getPlayerPos() const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void drawText(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    Context m_context;

    Player::Ptr m_player;

    GoodWorld m_world;
    Hell m_hell;

    bool m_isHell;
    bool m_needsChange;

    bool m_finished;

    Room::Ptr m_currentRoom;
};

#endif // GAME_H
