#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <cassert>

class Animation
{
public:
    Animation(unsigned int initialFrame, unsigned int frameNumber,
              sf::Time duration, bool repeat, const sf::Vector2i& size,
              const sf::Texture& texture);

    void update(sf::Time eTime);

    void restart();

    sf::IntRect getTextureRect();

    bool isFinished() const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    sf::Vector2i getFramePosition(unsigned int frame);

private:
    const sf::Texture& m_texture;

    sf::IntRect m_textureRect;

    const unsigned int m_initialFrame;
    const unsigned int m_frameNumber;
    unsigned int       m_currentFrame;

    const sf::Time m_duration;
    sf::Time       m_elapsedTime;

    const sf::Vector2i m_frameSize;

    const bool m_repeat;
};

#endif // ANIMATION_H

