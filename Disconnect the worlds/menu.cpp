#include "menu.h"

Menu::Menu(Context &context)
{
    m_finished = false;

    m_title.setFont(context.fontLoader->getResource(FontID::MENU));
    m_title.setCharacterSize(60.0f);
    m_title.setString("Disconnect the worlds");
    m_title.setPosition(40.0f, 10.0f);

    m_subtitle.setFont(context.fontLoader->getResource(FontID::MAIN));
    m_subtitle.setCharacterSize(30.0f);
    m_subtitle.setString(">What are you waiting for??\n\n>Click Action to start playing!");
    m_subtitle.setPosition(250.0f, 300.0f);

    m_name.setFont(context.fontLoader->getResource(FontID::MENU));
    m_name.setCharacterSize(20.0f);
    m_name.setString("DhantonGames");
    m_name.setPosition(830.0f, 650.0f);
}

void Menu::update(sf::Time eTime)
{

}

void Menu::handleInput(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::X)
        {
            m_finished = true;
        }
    }
}

bool Menu::isFinished() const
{
    return m_finished;
}

void Menu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.setView(target.getDefaultView());

    target.draw(m_title, states);
    target.draw(m_subtitle, states);
    target.draw(m_name, states);
}
