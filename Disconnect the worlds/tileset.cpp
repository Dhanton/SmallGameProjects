#include "tileset.h"

TileSet::TileSet(Context &context, TextureID id):
    m_texture(context.textureLoader->getResource(id))
{
    m_tileset.setPrimitiveType(sf::Quads);
}

void TileSet::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.texture = &m_texture;

    target.draw(m_tileset, states);
}

void TileSet::addVertex(float worldPosX, float worldPosY, float texPosX, float texPosY)
{
    sf::Vertex vertex;

    vertex.position = {worldPosX, worldPosY};
    vertex.texCoords = {texPosX, texPosY};

    m_tileset.append(vertex);
}
