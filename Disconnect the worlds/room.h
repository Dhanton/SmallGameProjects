#ifndef ROOM_H
#define ROOM_H

#include <memory>

#include "tileset.h"
#include "player.h"
#include "boss.h"
#include "blueboss.h"
#include "greenboss.h"
#include "redboss.h"
#include "finalboss.h"
#include "minemanager.h"
#include "world.h"

class Room : public TileSet
{
public:
    enum class TileType
    {
        WALL_1,
        WALL_2,
        FLOOR
    };

    typedef std::unique_ptr<Room> Ptr;

public:
    Room(Context& context, Player* player, CardType type, bool isFinal = false);

    void update(sf::Time eTime, Game* game);
    void handleInput(const sf::Event& event);

    bool isFinalFinished() const;

    void loadRoom();

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void restart();

    void addTile(const sf::Vector2i pos, TileType type);

    void updateEndExplosions(sf::Time eTime);

    sf::Vector2u toTexturePos(TileType type);

private:
    Player* m_player;

    BulletManager    m_bulletMan;
    ExplosionManager m_explosionMan;
    EnemyManager     m_enemyMan;
    MineManager      m_mineMan;

    Boss::Ptr m_boss;

    bool m_isFinal;
    bool m_exit;

    CardType m_type;

    bool     m_endExplosions;
    sf::Time m_explosionElapsed;
    int      m_explosionCount;

    sf::Time m_restartTime;
};

#endif // ROOM_H
