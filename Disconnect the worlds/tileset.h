#ifndef TILESET_H
#define TILESET_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "context.h"

class TileSet : public sf::Drawable
{
public:
    TileSet(Context& context, TextureID id);

protected:
    void addVertex(float worldPosX, float worldPosY, float texPosX, float texPosY);

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

protected:
    sf::VertexArray m_tileset;

    const sf::Texture& m_texture;

};

#endif // TILESET_H
