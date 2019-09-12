#include "entity.hpp"
#include "entitymanager.hpp"
#include "map.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>

Entity::Entity(TextureLoader *tex, LightManager* lights, Vector2 pos, Type type, Vector2 speed)
{
    setPosition(pos);
    m_speed = speed;
    m_dead = false;
    m_type = type;

    m_tex = tex;

    m_light = nullptr;

    if (type == BULLET) {
        setTexture(m_tex->getResource("BULLET"));

        m_light = new Light(45.f, 15.f, {0.9f, 1.f, 1.f}, {0.f, 1.f}, M_PI/2);
        lights->addLight(m_light);
        m_light->setPosition(getCenter());
    }

    else if (type == MISSILE) {
        setTexture(m_tex->getResource("MISSILE"));

        m_light = new Light(45.f, 15.f, {0.7f, 0.2f, 0.2f}, {0.f, 1.f}, M_PI/2);
        lights->addLight(m_light);
        m_light->setPosition(getCenter());
    }

    else if (type == MONSTER) {
        setTexture(m_tex->getResource("MONSTER"));
        health = 5;
        float delta1 = static_cast<float>(rand()%5)/2.f;
        float delta2 = static_cast<float>(rand()%4 + 1)/2.f;
        m_timer2 = sf::seconds(delta1);
        m_timer = sf::seconds(delta2);
    }

    else if (type == WORM) {
        setTexture(m_tex->getResource("WORM"));
        health = 2;
    }

    else if (type == SOUL) {
        //m_light = new Light(45.f, 15.f, {0.9f, 1.f, 1.f}, {0.f, 1.f}, M_PI/2);
        //lights->addLight(m_light);
        //m_light->setPosition(getCenter());
        setTexture(m_tex->getResource("SOUL"));
    }

    m_timer2 = sf::seconds(2.5f);
}

void Entity::update(sf::Time eTime, Map* map)
{
    bool collidingWall = false;
    float dt = eTime.asSeconds();

    sf::FloatRect baseRect = getCollisionRect();
    sf::FloatRect xRect = baseRect;
    sf::FloatRect yRect = baseRect;

    xRect.left += m_speed.x * dt;
    yRect.top += m_speed.y * dt;

    if (m_type == BULLET || m_type == MISSILE) {
        if (map->isColliding(xRect) || map->isColliding(yRect)) {
            m_dead = true;
            return;
        }
    }

    if (map->isColliding(xRect)) {
        m_speed.x = 0.f;
        collidingWall = true;
    }

    if (map->isColliding(yRect)) {
        m_speed.y = 0.f;
        collidingWall = true;
    }

    move(m_speed * dt);

    //It's a prototype dont kill me
    m_timer += eTime;
    m_timer2 += eTime;
    m_timer3 += eTime;
    m_timer4 += eTime;

    Player* player = m_manager->getPlayer();
    sf::FloatRect playerRect = player->getCollisionRect();

    if (m_type == MONSTER) {
        if (m_timer >= sf::seconds(2.5f)) {
            float delta = static_cast<float>(rand()%4 + 1)/2.f;
            m_timer = sf::seconds(delta);

            Vector2 center = {getPosition().x + getTextureRect().width/2, getPosition().y + getTextureRect().height/2};
            Vector2 playerPos = m_manager->getPlayer()->getCenter();
            Vector2 result = {playerPos.x - center.x, playerPos.y - center.y};

            float length = std::sqrt(result.x * result.x + result.y * result.y);

            m_manager->addEntity(Entity::MISSILE, center, (result*230.f)/length);
        }

        if (collidingWall || m_timer2 >= sf::seconds(2.5f)) {
            float delta = static_cast<float>(rand()%5)/2.f;
            m_timer2 = sf::seconds(delta);

            int dir = rand() % 4;
            float vel = 100.f;

            m_speed = {0.f, 0.f};

            if (dir == 0) m_speed.x += vel;
            if (dir == 1) m_speed.x -= vel;
            if (dir == 2) m_speed.y += vel;
            if (dir == 3) m_speed.y -= vel;
        }
    }

    if (m_type == WORM) {
        Vector2 center = {getPosition().x + getTextureRect().width/2, getPosition().y + getTextureRect().height/2};
        Vector2 playerPos = m_manager->getPlayer()->getCenter();
        Vector2 result = {playerPos.x - center.x, playerPos.y - center.y};

        float length = std::sqrt(result.x * result.x + result.y * result.y);
        float vel = 50.f;

        if (length >= 300.f) {
            vel = 100.f;

            if (collidingWall || m_timer2 >= sf::seconds(2.5f)) {
                float delta = static_cast<float>(rand()%5)/2.f;
                m_timer2 = sf::seconds(delta);

                int dir = rand() % 4;
                float vel = 100.f;

                m_speed = {0.f, 0.f};

                if (dir == 0) m_speed.x += vel;
                if (dir == 1) m_speed.x -= vel;
                if (dir == 2) m_speed.y += vel;
                if (dir == 3) m_speed.y -= vel;
            }
        } else {
            m_speed = (result * vel)/length;
        }

    }

    if (m_type == BULLET) {
        Entity* entity = m_manager->checkCollision(getGlobalBounds());

        if (entity != nullptr && entity->getType() != SOUL) {
            entity->health -= 1;

            if (entity->health == 0) entity->kill();

            m_dead = true;
        }
    }

    if (getGlobalBounds().intersects(playerRect)) {
        if (m_type == MISSILE) {
            m_dead = true;
            player->doDamage(2);
        }

        if (m_type == MONSTER || m_type == WORM) {
            player->doDamage(1);
        }

        if (m_type == SOUL) {
            m_dead = true;
            player->addFuel(4);
        }
    }


    if (m_light != nullptr) m_light->setPosition(getCenter());
}

void Entity::setManager(EntityManager *a)
{
    m_manager = a;
}

sf::FloatRect Entity::getCollisionRect() const
{
    auto bounds = getGlobalBounds();

    if (m_type == MONSTER) {
        bounds.left += 23.f;
        bounds.top += 31.f;
        bounds.width = 49.f;
        bounds.height = 68.f;
    }

    return bounds;
}

void Entity::onDeath()
{
    if (m_light != nullptr) m_light->destroyed = true;

    if (m_type == MONSTER || m_type == WORM) {
        m_manager->addEntity(SOUL, getCenter());
    }
}

bool Entity::isDead() const
{
    return m_dead;
}

void Entity::kill()
{
    m_dead = true;
}

Entity::Type Entity::getType() const
{
    return m_type;
}

Vector2 Entity::getCenter() const
{
    return {getPosition().x + getGlobalBounds().width/2, getPosition().y + getGlobalBounds().height/2};
}
