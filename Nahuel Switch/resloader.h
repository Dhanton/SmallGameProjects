#ifndef RESLOADER_H
#define RESLOADER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <map>
#include <memory>
#include <cassert>
#include <stdexcept>

template<typename Resource, typename Identifier>
class ResLoader : private sf::NonCopyable
{
public:
    void loadResource(const std::string& filename, Identifier id);

    Resource&       getResource(Identifier id);
    const Resource& getResource(Identifier id) const;

    /* This function is used for shaders and special casses */
    template<typename Parameter> void loadResource(const std::string& filename, Identifier id, const Parameter& param);

private:
    std::map<Identifier, std::unique_ptr<Resource>> m_resourceMap;

};

using TextureLoader = ResLoader<sf::Texture, std::string>;
using FontLoader = ResLoader<sf::Font, std::string>;

#include "resloader.inl"

#endif // RESLOADER_H

