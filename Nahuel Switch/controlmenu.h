#ifndef CONTROLMENU_H
#define CONTROLMENU_H

#include "stage.h"
#include "context.h"
#include "player.h"

class ControlMenu : public BaseContext, public Stage
{
public:
    ControlMenu(Context context, StageManager* parent);

    void update(sf::Time eTime);
    void handleInput(const sf::Event& event);

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    static constexpr size_t m_playerMax = 6;

    bool m_canStart;

    std::vector<PlayerInfo> m_keyBindings;

    std::vector<sf::Color> m_colors;

    static std::map<sf::Keyboard::Key, std::string> m_keyStrings;
};

#endif // CONTROLMENU_H
