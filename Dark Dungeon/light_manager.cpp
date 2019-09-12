#include "light_manager.hpp"

LightManager::LightManager(sf::Vector2u windowSize, sf::RenderTexture *normalMap)
{
    m_windowSize = windowSize;
    m_normalMap = normalMap;

    m_shader.loadFromFile("light_shader.frag", sf::Shader::Fragment);

    m_state.shader = &m_shader;
    m_state.blendMode = sf::BlendAdd;

    m_lightMap.create(windowSize.x, windowSize.y);
}

void LightManager::update(sf::Time eTime)
{
    for (size_t i = 0; i < m_lights.size(); ++i) {
        if (m_lights[i]->destroyed) {
            m_lights.erase(m_lights.begin() + i);
        }
    }
}

void LightManager::renderLights(sf::RenderTexture *normalMap, sf::View* view)
{
    m_lightMap.clear();

    //Check if the light is inside current view
    for (size_t i = 0; i < m_lights.size(); ++i) {
        Light* light = m_lights[i].get();

        if (light->destroyed) {
            m_lights.erase(m_lights.begin() + i);
            continue;
        }

        //We render a sprite in (0, 0) so we dont change the view, we update the light's pos
        //Zoom doesn't work with lights
        Vector2 cameraPos = {view->getCenter().x - view->getSize().x/2.f, view->getCenter().y - view->getSize().y/2.f};
        Vector2 finalPos = {light->getPosition().x - cameraPos.x, light->getPosition().y - cameraPos.y};

        sf::Sprite sprite;

        sprite.setTexture(normalMap->getTexture());

        m_shader.setUniform("position", finalPos);
        m_shader.setUniform("height", (float) m_windowSize.y);
        m_shader.setUniform("radius", light->radius);
        m_shader.setUniform("intensity", light->intensity);
        m_shader.setUniform("lightColor", light->color);
        m_shader.setUniform("direction", light->direction);
        m_shader.setUniform("angle", light->angle);
        m_shader.setUniform("nMaps", normalMap->getTexture());

        m_lightMap.draw(sprite, m_state);

    }

    m_lightMap.display();
}

void LightManager::clearLights()
{
    m_lights.clear();
}

void LightManager::addLight(Light* light)
{
    m_lights.push_back(std::move(Ptr<Light>(light)));
}

sf::RenderTexture *LightManager::getLightMap()
{
    return &m_lightMap;
}
