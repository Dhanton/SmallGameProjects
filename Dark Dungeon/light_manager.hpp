#ifndef LIGHT_MANAGER_HPP
#define LIGHT_MANAGER_HPP

#include <SFML/Graphics.hpp>

#include "defines.hpp"

struct Light : sf::Transformable{
    float radius;
    float intensity;

    sf::Vector3f color;
    Vector2 direction;
    float angle;

    bool destroyed;

    Light(float r, float i, sf::Vector3f c, Vector2 d, float a)
    {
        radius = r;
        intensity = i;
        color = c;
        direction = d;
        angle = a;

        destroyed = false;
    }
};

class LightManager
{
public:
    LightManager(sf::Vector2u windowSize, sf::RenderTexture* normalMap);

    void update(sf::Time eTime);

    void renderLights(sf::RenderTexture* normalMap, sf::View *view);

    void clearLights();

    void addLight(Light *light);
    sf::RenderTexture* getLightMap();

private:
    sf::Vector2u m_windowSize;

    sf::RenderTexture* m_normalMap;
    sf::RenderTexture m_lightMap;

    sf::Shader m_shader;
    sf::RenderStates m_state;

    Ptr_vector<Light> m_lights;
};

#endif // LIGHT_MANAGER_HPP
