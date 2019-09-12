#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include "entity.hpp"
#include "player.hpp"

class Map;

class EntityManager : public sf::Drawable
{
public:
    EntityManager(TextureLoader* tex, LightManager *lights, sf::RenderWindow *window, sf::RenderTarget *canvas, Map *map);

    void start();
    void update(sf::Time eTime);

    void addEntity(Entity::Type type, Vector2 pos, Vector2 speed = {0.f, 0.f});

    Player* getPlayer() const;
    Entity *checkCollision(const sf::FloatRect &rect) const;

    void setNormalRender(bool a);
private:
    TextureLoader* m_tex;
    Map* m_map;
    LightManager* m_lights;

    Vector2 m_startingPos;

    Ptr<Player> m_player;
    Ptr_vector<Entity> m_entities;
    Ptr_vector<Entity> m_bullets;

    bool m_renderNormal;
    static const std::string m_normalShader;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif // ENTITYMANAGER_HPP
