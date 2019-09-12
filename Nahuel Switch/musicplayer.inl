#include "musicplayer.h"

template<typename Identifier>
MusicPlayer<Identifier>::MusicPlayer()
{
    m_volume = 100.0f;
}

template<typename Identifier>
void MusicPlayer<Identifier>::loadMusic(Identifier id, const std::string &filename)
{
    m_filenames[id] = filename;
}

template<typename Identifier>
void MusicPlayer<Identifier>::play(Identifier theme)
{
    std::string filename = m_filenames[theme];

    if (!m_music.openFromFile(filename))
        throw std::runtime_error("Music player - Filename " + filename +
                                 " could not be loaded\n");

    m_music.setVolume(m_volume);
    m_music.setLoop(true);
    m_music.play();
}

template<typename Identifier>
void MusicPlayer<Identifier>::setPaused(bool paused)
{
    (paused ? m_music.pause() : m_music.play());
}

template<typename Identifier>
void MusicPlayer<Identifier>::setVolume(float volume)
{
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 100.0f) volume = 100.0f;

    m_volume = volume;
}

template<typename Identifier>
sf::Music::Status MusicPlayer<Identifier>::getStatus()
{
    return m_music.getStatus();
}

