#include "boss.h"

Boss::Boss(Context& context, TextureID id):
    m_animation(context.textureLoader->getResource(id), {16, 16})
{
    m_dead = false;
}

void Boss::update(sf::Time eTime, EnemyManager &enemyMan, BulletManager &bMan, MineManager &mineMan)
{
    if (m_life == 0) m_dead = true;

    Entity::update(eTime);
}

sf::FloatRect Boss::getGlobalBounds()
{
    sf::FloatRect rect = getTransform().transformRect(m_animation.getSprite().getGlobalBounds());

    rect.left += 10.0f;
    rect.height += 10.0f;
    rect.width -= 20.0f;
    rect.height -= 20.0f;

    return rect;
}

void Boss::takeDamage()
{
    if (m_life == 0) return;

    m_life--;
}

void Boss::respawn()
{
    m_dead = false;
}

bool Boss::isDead() const
{
    return m_dead;
}

void Boss::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_animation, states);
}
