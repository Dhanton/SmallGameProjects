#include "explosionmanager.h"

ExplosionManager::ExplosionManager(Context& context):
    m_texture(context.textureLoader->getResource(TextureID::OTHERS)),
    m_duration(sf::milliseconds(300.0f)),
    m_soundPlayer(context.soundPlayer)
{
}

void ExplosionManager::update(sf::Time eTime)
{
    for (auto& explosion : m_explosions)
    {
        explosion->update(eTime);
    }

    m_explosions.erase(std::remove_if(m_explosions.begin(), m_explosions.end(),
                                      [&] (std::unique_ptr<AnimationManager> const &anim){return anim->getCurrentAnimation()->isFinished();}),
                                        m_explosions.end());
}

void ExplosionManager::addExplosion(const sf::Vector2f &position)
{
    std::unique_ptr<AnimationManager> expl{new AnimationManager(m_texture, {16, 16})};

    expl->setPosition(position);
    expl->addAnimation("MAIN", 20, 4, m_duration, false);
    expl->playAnimation("MAIN");

    expl->getSprite().setScale(2.0f, 2.0f);
    m_explosions.push_back(std::move(expl));

    m_soundPlayer->play(SoundID::EXPLOSION, position);
}

void ExplosionManager::clear()
{
    m_explosions.clear();
}

bool ExplosionManager::isEmpty() const
{
    return m_explosions.empty();
}

void ExplosionManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto& explosion : m_explosions)
    {
        target.draw(*explosion, states);
    }
}
