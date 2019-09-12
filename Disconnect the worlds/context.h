#ifndef CONTEXT_H
#define CONTEXT_H

#include <SFML/Graphics.hpp>

#include "resloader.h"
#include "soundplayer.h"

struct Context
{
    Context()
    {

    }

    sf::RenderWindow* window;
    TextureLoader* textureLoader;
    FontLoader* fontLoader;
    SoundPlayer<>* soundPlayer;
};

#endif // CONTEXT_H
