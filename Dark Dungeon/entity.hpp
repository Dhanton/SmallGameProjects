#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

#include "res_loader.hpp"
#include "defines.hpp"
#include "light_manager.hpp"

class EntityManager;
class Map;

class Entity : public sf::Sprite
{
public:
    enum Type {
        BULLET,
        MISSILE,
        MONSTER,
        WORM,
        SOUL
    };

    int health;

public:
    Entity(TextureLoader* tex, LightManager *lights, Vector2 pos, Type type, Vector2 speed = {0.f, 0.f});

    void update(sf::Time eTime, Map *map);

    void setManager(EntityManager* a);

    sf::FloatRect getCollisionRect() const;

    void onDeath();

    bool isDead() const;
    void kill();

    Type getType() const;
    Vector2 getCenter() const;

private:
    TextureLoader* m_tex;
    EntityManager* m_manager;

    Vector2 m_speed;
    Type m_type;

    bool m_dead;

    Light* m_light;

    sf::Time m_timer;
    sf::Time m_timer2;
    sf::Time m_timer3;
    sf::Time m_timer4;
};

#endif // ENTITY_HPP
