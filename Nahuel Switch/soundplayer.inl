#include "soundplayer.h"

template<typename Identifier>
SoundPlayer<Identifier>::SoundPlayer()
{

}

template<typename Identifier>
void SoundPlayer<Identifier>::loadSound(Identifier id, const std::string &filename)
{
    m_soundBuffer.loadResource(filename, id);
}

template<typename Identifier>
void SoundPlayer<Identifier>::play(Identifier id)
{
    play(id, getListenerPosition());
}

template<typename Identifier>
void SoundPlayer<Identifier>::play(Identifier id, Vector2 position)
{
    m_sounds.push_back(sf::Sound());

    sf::Sound& sound = m_sounds.back();

    sound.setBuffer(m_soundBuffer.getResource(id));
    sound.setPosition(position.x, -position.y, 0.0f);
    sound.setAttenuation(attenuation);
    sound.setMinDistance(minDistance3);

    sound.play();
}

template<typename Identifier>
void SoundPlayer<Identifier>::removeStoppedSounds()
{
    m_sounds.remove_if([] (const sf::Sound& sound)
    {
        return sound.getStatus() == sf::Sound::Stopped;
    });
}

template<typename Identifier>
void SoundPlayer<Identifier>::setListenerPosition(Vector2 position)
{
    sf::Listener::setPosition(position.x, -position.y, listenerZ);
}

template<typename Identifier>
Vector2 SoundPlayer<Identifier>::getListenerPosition() const
{
    Vector2 vec;

    vec.x = sf::Listener::getPosition().x;
    vec.y = -sf::Listener::getPosition().y;

    return vec;
}

