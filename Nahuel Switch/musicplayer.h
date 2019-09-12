#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <SFML/Audio.hpp>
#include <stdexcept>
#include <map>

template<typename Identifier>
class MusicPlayer : private sf::NonCopyable
{
public:
    MusicPlayer();

    void loadMusic(Identifier id, const std::string& filename);

    void play(Identifier theme);
    void stop();

    void setPaused(bool paused);
    void setVolume(float volume);

    sf::Music::Status getStatus();

private:
    std::map<Identifier, std::string> m_filenames;

    sf::Music m_music;

    float m_volume;
};

#include "musicplayer.inl"

#endif // MUSICPLAYER_H

