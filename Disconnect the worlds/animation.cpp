#include "animation.h"

Animation::Animation(unsigned int initialFrame, unsigned int frameNumber,
                     sf::Time duration, bool repeat, const sf::Vector2i &size,
                     const sf::Texture &texture):
    m_texture(texture),
    m_initialFrame(initialFrame),
    m_frameNumber(frameNumber),
    m_duration(duration),
    m_frameSize(size),
    m_repeat(repeat)
{
    assert(m_frameNumber != 0);

    restart();

    m_textureRect = {getFramePosition(m_initialFrame), m_frameSize};
}

void Animation::update(sf::Time eTime)
{
    const sf::Time timePerFrame = m_duration / static_cast<float>(m_frameNumber);
    const unsigned int maxFrame = m_frameNumber + m_initialFrame;

    m_elapsedTime += eTime;

    if (m_elapsedTime >= timePerFrame && (m_repeat || m_currentFrame < maxFrame))
    {
        //m_elapsedTime = sf::Time::Zero; - Alternative design
        m_elapsedTime -= timePerFrame;

        m_currentFrame++;

        if (m_currentFrame >= maxFrame)
        {
            if (m_repeat) m_currentFrame = m_initialFrame;
            else return;
        }
    }

    m_textureRect.left = getFramePosition(m_currentFrame).x;
    m_textureRect.top  = getFramePosition(m_currentFrame).y;
}

void Animation::restart()
{
    m_currentFrame = m_initialFrame;
    m_elapsedTime = sf::Time::Zero;
}

sf::IntRect Animation::getTextureRect()
{
    return m_textureRect;
}

bool Animation::isFinished() const
{
    return (!m_repeat && m_currentFrame >= m_frameNumber + m_initialFrame);
}

sf::Vector2i Animation::getFramePosition(unsigned int frame)
{
    unsigned int textureWidth = m_texture.getSize().x;

    unsigned int xFrames = textureWidth / m_frameSize.x;

    sf::Vector2i frameCoords = {frame % xFrames, frame / xFrames};

    return {frameCoords.x * m_frameSize.x, frameCoords.y * m_frameSize.y};
}

