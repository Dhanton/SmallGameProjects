#ifndef STAGE_H
#define STAGE_H

#include <SFML/Graphics.hpp>
#include <memory>

class StageManager;

class Stage : public sf::Drawable
{
public:
    using Ptr = std::unique_ptr<Stage>;
public:
    Stage(StageManager* parent):m_parent(parent) {}

    virtual void update(sf::Time eTime) = 0;
    virtual void handleInput(const sf::Event& event) = 0;

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;

protected:
    StageManager* m_parent;
};

#endif // STAGE_H
