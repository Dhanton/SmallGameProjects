#include "player.hpp"
#include "entitymanager.hpp"

#include <iostream>

#include "map.hpp"
#include <cmath>

template<typename T>
T sgn(T a) {
    if (a == 0) return 0;

    return a/std::abs(a);
}

const std::string Player::m_invinShader = \
    "uniform bool timer;" \
    "uniform sampler2D texture;" \
    "void main()" \
    "{" \
    "   vec4 pixel = texture2D(texture, gl_TexCoord[0].st).rgba;" \
    "   if (timer) pixel = vec4(1.0, 0.0, 0.0, pixel.a);" \
    "   gl_FragColor = pixel;" \
    "}";

Player::Player(TextureLoader* tex, LightManager *lights, sf::RenderWindow *window, sf::RenderTarget* canvas, Vector2 pos)
{
    setPosition(pos);
    m_tex = tex;
    m_lights = lights;
    m_window = window;
    m_canvas = canvas;


    setTexture(m_tex->getResource("PLAYER"));
    m_canShoot = true;

    //reset();
}

void Player::reset()
{
    m_canShoot = true;
    m_timer = sf::Time::Zero;

    m_maxHealth = 8;
    m_maxFuel = 32;
    m_fuel = m_maxFuel;
    m_health = m_maxHealth;
    m_dead = false;
    m_invinFrame = false;

    //Example light 2174.28 1913.5 radius=128 intensity=8 1 1 1
    m_candle = new Light(300.f, 75.f, {1.f, 1.f, 1.f}, {0.f, 1.f}, M_PI/2);
    m_lights->addLight(m_candle);

    sf::Vector2i mousePixel = sf::Mouse::getPosition(*m_window);
    sf::Vector2f mousePos = m_canvas->mapPixelToCoords(mousePixel);
    updateCandle(mousePos);
}

void Player::update(sf::Time eTime, Map* m_map)
{
    float dt = eTime.asSeconds();

    sf::FloatRect baseRect = getTileRect();
    sf::FloatRect xRect = baseRect;
    sf::FloatRect yRect = baseRect;

    xRect.left += m_speed.x * dt;
    yRect.top += m_speed.y * dt;

    if (m_map->isColliding(xRect)) {
//        for (float i = 0.0f; i < std::abs(m_speed.x * dt); i += 0.1f) {
//            sf::FloatRect resultingRect = baseRect;

//            resultingRect.left += 0.1f * sgn(m_speed.x);

//            if (!m_map->isColliding(resultingRect)) {
//                move(0.1f * sgn(m_speed.x), 0.0f);
//                continue;
//            }

//            m_speed.x = 0.0f;
//            break;
//        }
        m_speed.x = 0.f;
      }

    if (m_map->isColliding(yRect)) {
//        for (float i = 0.0f; i < std::abs(m_speed.y * dt); i += 0.1f) {
//            sf::FloatRect resultingRect = baseRect;

//            resultingRect.top +=  0.1f * sgn(m_speed.y);

//            if (!m_map->isColliding(resultingRect)) {
//                move(0.f, 0.1f * sgn(m_speed.y));
//                continue;
//            }

//            m_speed.y = 0.0f;
//            break;
//        }
        m_speed.y = 0.f;
      }

    move(m_speed * dt);

    m_timer += eTime;

    if (m_timer >= sf::seconds(0.5f)) {
        m_timer = sf::Time::Zero;
        m_canShoot = true;
    }

    if (m_invinFrame) {
        m_invinTimer += eTime;
        m_shaderTimer += eTime;

        if (m_invinTimer >= sf::seconds(2.f)) {
            m_invinFrame = false;
            m_invinTimer = sf::Time::Zero;
            m_shaderTimer = sf::Time::Zero;
        }

        if (m_shaderTimer >= sf::milliseconds(300)) {
            m_shaderTimer = sf::Time::Zero;
        }
    }

    sf::Vector2i mousePixel = sf::Mouse::getPosition(*m_window);
    sf::Vector2f mousePos = m_canvas->mapPixelToCoords(mousePixel);
    updateCandle(mousePos);
}

void Player::handleInput(const sf::Event &event, const sf::RenderWindow& window)
{
    float speed = 250.0f;

    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) m_speed.x = -speed;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) m_speed.x = speed;
    else m_speed.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) m_speed.y = -speed;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) m_speed.y = speed;
    else m_speed.y = 0.f;

    if (m_fuel > 0 && m_canShoot && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = m_canvas->mapPixelToCoords(mousePixel);
        Vector2 center = getCenter();
        float bulletSpeed = 500.f + std::sqrt(m_speed.x * m_speed.x + m_speed.y * m_speed.y); //Whatever

        Vector2 result = {mousePos.x - center.x, mousePos.y - center.y};
        float length = std::sqrt(result.x * result.x + result.y * result.y);
        Vector2 unitary = result/length;

        m_manager->addEntity(Entity::BULLET, center, unitary * bulletSpeed);

        m_canShoot = false;
        m_fuel -= 1;
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
        m_dead = true;
    }
}

void Player::renderBars(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::View oldView = target.getView();
    target.setView(target.getDefaultView());

    sf::RectangleShape healthBar;
    healthBar.setPosition(20.f, 20.f);
    healthBar.setSize({160.f, 20.f});
    healthBar.setFillColor(sf::Color::Black);
    healthBar.setOutlineColor(sf::Color::White);
    healthBar.setOutlineThickness(3.f);

    sf::RectangleShape health;
    health.setPosition(healthBar.getPosition().x, healthBar.getPosition().y);
    health.setSize({20.f*m_health, 20.f});
    health.setFillColor(sf::Color(0, 159, 138));

    sf::RectangleShape fuelBar;
    fuelBar.setPosition(20.f, 50.f);
    fuelBar.setSize({160.f, 20.f});
    fuelBar.setFillColor(sf::Color::Black);
    fuelBar.setOutlineColor(sf::Color::White);
    fuelBar.setOutlineThickness(3.f);

    sf::RectangleShape fuel;
    fuel.setPosition(fuelBar.getPosition().x, fuelBar.getPosition().y);
    fuel.setSize({static_cast<float>(160.f/m_maxFuel)*m_fuel, 20.f});
    fuel.setFillColor(sf::Color(228, 237, 79));


    target.draw(healthBar, states);
    target.draw(health, states);
    target.draw(fuelBar, states);
    target.draw(fuel, states);

    target.setView(oldView);
}

void Player::setManager(EntityManager *a)
{
    m_manager = a;
}

void Player::doDamage(int amount)
{
    if (m_invinFrame) return;

    m_health -= amount;

    if (m_health <= 0) m_dead = true;

    m_invinFrame = true;

}

void Player::addFuel(int amount)
{
    m_fuel += amount;

    if (m_fuel > m_maxFuel) m_fuel = m_maxFuel;
}

Vector2 Player::getCenter() const
{
    return {getPosition().x + getGlobalBounds().width/2, getPosition().y + getGlobalBounds().height/2};
}

sf::FloatRect Player::getCollisionRect() const
{
    auto bounds = getGlobalBounds();
    float delta = 55.f;

    bounds.left += delta;
    bounds.top += delta;
    bounds.width -= delta*2;
    bounds.height -= delta+35.f;

    return bounds;
}

sf::FloatRect Player::getTileRect() const
{
    auto bounds = getGlobalBounds();
    float delta = 55.f;

    bounds.left += delta;
    bounds.top += delta+25.f;
    bounds.width -= delta*2;
    bounds.height -= delta+50.f;

    return bounds;
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::updateCandle(Vector2 mousePos)
{
    Vector2 result = {mousePos.x - getCenter().x, mousePos.y - getCenter().y};
    float length = std::sqrt(result.x * result.x + result.y * result.y);

    m_candle->direction = -result/length;
    m_candle->setPosition(getCenter());

    //    m_candle->setPosition({getCenter().x, getCenter().y + 100.f});
}

void Player::render(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Shader shader;

    if (m_invinFrame && states.shader == nullptr) {
        shader.loadFromMemory(m_invinShader, sf::Shader::Fragment);

        shader.setUniform("timer", (m_shaderTimer.asMilliseconds() <= 150.f));
        shader.setUniform("texture", sf::Shader::CurrentTexture);

        states.shader = &shader;
    }

    target.draw(*this, states);
}


