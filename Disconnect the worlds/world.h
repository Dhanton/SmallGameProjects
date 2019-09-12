#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>

#include "player.h"
#include "tilemap.h"
#include "obstaclemanager.h"
#include "enemymanager.h"
#include "explosionmanager.h"
#include "portal.h"

class Game;

class World : public sf::Drawable
{
public:
    enum class Type
    {
        GOOD,
        HELL
    };

public:
    World(Context& context, TextureID id);

    virtual void update(sf::Time eTime, Game *game);
    void handleInput(const sf::Event& event);

    void setPlayer(Player* player);

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

protected:
    SoundPlayer<>* m_soundPlayer;

    Type m_type;

    Player* m_player;

    TileMap         m_tileMap;
    ObstacleManager m_obs;
    BulletManager   m_bulletMan;

    Portal m_portal;

    bool m_shouldChangeWorlds;
};

#endif // WORLD_H
