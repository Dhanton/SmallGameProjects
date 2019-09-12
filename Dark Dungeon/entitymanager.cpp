#include "entitymanager.hpp"
#include "map.hpp"

const std::string EntityManager::m_normalShader = \
    "uniform sampler2D texture;" \
    "void main()" \
    "{" \
    "   float a = texture2D(texture, gl_TexCoord[0].st).a;" \
    "   if (a == 1.0) gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);" \
    "}";

EntityManager::EntityManager(TextureLoader *tex, LightManager* lights, sf::RenderWindow *window, sf::RenderTarget *canvas, Map* map)
{
    m_tex = tex;
    m_map = map;
    m_lights = lights;

    m_startingPos = map->getStartingPos();

    m_player = Ptr<Player>(new Player(m_tex, lights, window, canvas, m_startingPos));
    m_player->setManager(this);

    m_renderNormal = false;

    start();
}

void EntityManager::start()
{
    m_map->generateRandomly({60, 30}, 500);

    m_startingPos = m_map->getStartingPos();

    m_player->setPosition(m_startingPos);
    m_player->reset();

    m_entities.clear();
    m_bullets.clear();

    m_map->spawnEnemies(this);
}

void EntityManager::update(sf::Time eTime)
{
    m_player->update(eTime, m_map);

    for (size_t i = 0; i < m_entities.size(); ++i) {
        m_entities[i]->update(eTime, m_map);

        if (m_entities[i]->isDead()) {
            m_entities[i]->onDeath();
            m_entities.erase(m_entities.begin() + i);
        }
    }

    for (size_t i = 0; i < m_bullets.size(); ++i) {
        m_bullets[i]->update(eTime, m_map);

        if (m_bullets[i]->isDead()) {
            m_bullets[i]->onDeath();
            m_bullets.erase(m_bullets.begin() + i);
        }
    }

    m_lights->update(eTime);

    if (m_player->isDead()) {
        m_lights->clearLights();
        start();
    }
}

void EntityManager::addEntity(Entity::Type type, Vector2 pos, Vector2 speed)
{
    Ptr<Entity> a = Ptr<Entity>(new Entity(m_tex, m_lights, pos, type, speed));

    a->setManager(this);

    if (type == Entity::BULLET || type == Entity::MISSILE) {
        m_bullets.push_back(std::move(a));

    } else {
        m_entities.push_back(std::move(a));

    }
}

Player *EntityManager::getPlayer() const
{
    return m_player.get();
}

Entity* EntityManager::checkCollision(const sf::FloatRect& rect) const
{
    for (const auto& entity : m_entities) {
        if (entity->getGlobalBounds().intersects(rect)) {
            return entity.get();
        }
    }

    return nullptr;
}

void EntityManager::setNormalRender(bool a)
{
    m_renderNormal = a;
}

void EntityManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Shader shader;

    if (m_renderNormal) {
        shader.loadFromMemory(m_normalShader, sf::Shader::Fragment);
        shader.setUniform("texture", sf::Shader::CurrentTexture);
        states.shader = &shader;
    }

    for (auto& entity : m_entities)
        target.draw(*entity, states);

    for (auto& entity : m_bullets)
        target.draw(*entity, states);

    //target.draw(*m_player, states);
    m_player->render(target, states);

    //if (!m_renderNormal) {
       //m_player->renderBars(target, states);
    //}
}
