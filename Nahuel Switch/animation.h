#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <cassert>

class Animation
{
public:
    Animation(size_t initialFrame, size_t frameNumber,
              sf::Time duration, bool repeat, const sf::Vector2<size_t>& size,
              const sf::Texture& texture);

    void update(sf::Time eTime);

    void restart();

    sf::IntRect getTextureRect();

    bool isFinished() const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    sf::Vector2<size_t> getFramePosition(size_t frame);

private:
    const sf::Texture& m_texture;

    sf::IntRect m_textureRect;

    const size_t m_initialFrame;
    const size_t m_frameNumber;
    size_t       m_currentFrame;

    const sf::Time m_duration;
    sf::Time       m_elapsedTime;

    const sf::Vector2<size_t> m_frameSize;

    const bool m_repeat;
};

#endif // ANIMATIONFRAME_H

