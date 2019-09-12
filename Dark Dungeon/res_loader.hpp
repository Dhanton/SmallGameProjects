#ifndef RES_LOADER_HPP
#define RES_LOADER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <map>
#include <memory>
#include <cassert>
#include <stdexcept>

#include "defines.hpp"

template <typename Res, typename Id>
class ResLoader : private sf::NonCopyable
{
public:
    void loadResource(const std::string& filename, Id id);

    Res& getResource(Id id);
    const Res& getResource(Id id) const;

    template <typename Param> void loadResource(const std::string& filename, Id id, const Param &param);
private:
    std::map<Id, Ptr<Res>> m_resourceMap;
};

using TextureLoader = ResLoader<sf::Texture, std::string>;

#include "res_loader.inl"

#endif // RES_LOADER_HPP
