#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "res_loader.hpp"
#include "defines.hpp"
#include "light_manager.hpp"

class EntityManager;
class Map;

class Player : public sf::Sprite
{
public:
    Player(TextureLoader *tex, LightManager* lights, sf::RenderWindow *window, sf::RenderTarget *canvas, Vector2 pos);

    void reset();

    void update(sf::Time eTime, Map *m_map);
    void handleInput(const sf::Event& event, const sf::RenderWindow &window);

    void renderBars(sf::RenderTarget &target, sf::RenderStates states = sf::RenderStates::Default) const;

    void setManager(EntityManager* a);

    void doDamage(int amount);
    void addFuel(int amount);

    Vector2 getCenter() const;
    sf::FloatRect getCollisionRect() const;
    sf::FloatRect getTileRect() const;

    bool isDead() const;
    void render(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    void updateCandle(Vector2 mousePos);

    TextureLoader* m_tex;
    EntityManager* m_manager;
    LightManager* m_lights;
    sf::RenderWindow* m_window;
    sf::RenderTarget* m_canvas;

    Light* m_candle;

    Vector2 m_speed;

    int m_health;
    int m_maxHealth;
    int m_fuel;
    int m_maxFuel;
    bool m_dead;

    sf::Time m_invinTimer;
    bool m_invinFrame;
    static const std::string m_invinShader;
    sf::Time m_shaderTimer;

    sf::Time m_timer;
    bool m_canShoot;
};

#endif // PLAYER_HPP
