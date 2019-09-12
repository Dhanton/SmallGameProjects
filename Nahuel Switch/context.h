#ifndef CONTEXT
#define CONTEXT

#include "resloader.h"

struct Context
{
    Context(TextureLoader* t, FontLoader* f, sf::RenderWindow* w)
    {
        textures = t;
        fonts = f;
        window = w;
    }

    TextureLoader* textures;
    FontLoader* fonts;
    sf::RenderWindow* window;
};

class BaseContext
{
public:
    BaseContext(Context c):m_context(c) {}

protected:
    Context m_context;
};

#endif // CONTEXT

