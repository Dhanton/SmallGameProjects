#include "animationmanager.h"

AnimationManager::AnimationManager(const sf::Texture& texture, const sf::Vector2i size):
    m_size(size)
{
    m_sprite.setTexture(texture);

    m_currentAnimation = nullptr;
}

void AnimationManager::update(sf::Time eTime)
{
    if (m_currentAnimation == nullptr) return;

    m_currentAnimation->update(eTime);

    m_sprite.setTextureRect(m_currentAnimation->getTextureRect());
}

void AnimationManager::addAnimation(const std::string &name, unsigned int initialFrame, unsigned int frameNumber,
                                    sf::Time duration, bool repeat)
{
    Animation animation(initialFrame, frameNumber, duration, repeat, m_size, *m_sprite.getTexture());

    auto inserted = m_animationMap.insert(std::make_pair(name, std::move(animation)));

    assert(inserted.second);
}

void AnimationManager::playAnimation(const std::string &name)
{
    auto found = m_animationMap.find(name);
    assert(found != m_animationMap.end());

    if (m_currentAnimation == &found->second) return;

    m_currentAnimation = &found->second;

    m_currentAnimation->restart();
    m_currentAnimation->update(sf::Time::Zero);

    m_sprite.setTextureRect(m_currentAnimation->getTextureRect());
}

Animation *AnimationManager::getCurrentAnimation() const
{
    return m_currentAnimation;
}

sf::Sprite &AnimationManager::getSprite()
{
    return m_sprite;
}

sf::FloatRect AnimationManager::getSpriteBounds() const
{
    return getTransform().transformRect(m_sprite.getGlobalBounds());
}

void AnimationManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_sprite, states);
}

