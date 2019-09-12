#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <base64.h>
#include <zlib.h>

#include "tilemap.h"
#include "context.h"
#include "player.h"

class Level : public BaseContext, public sf::Drawable
{
public:
    Level(Context context, const std::vector<PlayerInfo> &playersInfo);

    void update(sf::Time eTime);
    void handleInput(const sf::Event& event);

    void loadLevel(const std::string &filename);
    void buildLevel(size_t chunkNumber);

    void respawnPlayers();

    /* Getters */

    std::vector<PlayerScore> getPlayerScore(float ratio = 10.0f) const;

    size_t getPlayerNumber() const;

    bool isFinished() const;

    float getScrollingVel() const;

    sf::View getView() const;

private:
    void loadLayer(rapidxml::xml_node<>* parent, const std::string &filename);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::View m_view;

    float m_scrollingVel;
    float m_finalPos;

    Vector2 m_initialPos;

    std::string m_textureID;

    size_t m_tileSize;

    std::vector<TileMap*> m_chunks;
    std::vector<TileMap>  m_patterns;

    std::vector<Player::Ptr> m_players;
};

#endif // LEVEL_H
