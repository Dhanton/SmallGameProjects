#include "resloader.h"

template<typename Resource, typename Identifier>
void ResLoader<Resource, Identifier>::loadResource(const std::string &filename, Identifier id)
{
    std::unique_ptr<Resource> resource(new Resource());

    if (!resource->loadFromFile(filename))
        throw std::runtime_error("ResourceHandler::loadResource - Failed to load " + filename + '\n');

    auto inserted = m_resourceMap.insert(std::make_pair(id, std::move(resource)));

    assert(inserted.second);
}

template<typename Resource, typename Identifier>
Resource &ResLoader<Resource, Identifier>::getResource(Identifier id)
{
    auto found = m_resourceMap.find(id);
    assert(found != m_resourceMap.end());

    return *found->second;
}

template<typename Resource, typename Identifier>
const Resource &ResLoader<Resource, Identifier>::getResource(Identifier id) const
{
    auto found = m_resourceMap.find(id);
    assert(found != m_resourceMap.end());

    return *found->second;
}

template<typename Resource, typename Identifier>
template<typename Parameter>
void ResLoader<Resource, Identifier>::loadResource(const std::string &filename, Identifier id, const Parameter &param)
{
    std::unique_ptr<Resource> resource(new Resource());

    if (!resource->loadFromFile(filename, param))
        throw std::runtime_error("ResourceHandler::loadResource - Failed to load " + filename + '\n');

    auto insterted = m_resourceMap.insert(std::make_pair(id, std::move(resource)));

    assert(insterted.second);
}

