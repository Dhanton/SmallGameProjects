#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <SFML/Audio.hpp>
#include <list>

#include "vector2.h"
#include "resloader.h"

namespace
{
const float listenerZ{300.0f};
const float attenuation{8.0f};
const float minDistance2{200.0f};
const float minDistance3{std::sqrt(minDistance2 * minDistance2 + listenerZ * listenerZ)};
}

template<typename Identifier>
class SoundPlayer : private sf::NonCopyable
{
public:
    SoundPlayer();

    void loadSound(Identifier id, const std::string& filename);

    void play(Identifier id);
    void play(Identifier id, Vector2 position);

    void removeStoppedSounds();

    void    setListenerPosition(Vector2 position);
    Vector2 getListenerPosition() const;


private:
    ResLoader<sf::SoundBuffer, Identifier> m_soundBuffer;
    std::list<sf::Sound> m_sounds;

};

#include "soundplayer.inl"

#endif // SOUNDPLAYER_H

