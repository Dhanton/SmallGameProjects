#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "context.h"

class Menu : public sf::Drawable
{
public:
    typedef std::unique_ptr<Menu> Ptr;

public:
    Menu(Context& context);

    void update(sf::Time eTime);

    void handleInput(const sf::Event& event);

    bool isFinished() const;

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Text m_title;
    sf::Text m_subtitle;
    sf::Text m_name;

    bool m_finished;
};

#endif // MENU_H
