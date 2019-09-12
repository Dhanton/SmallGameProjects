#include "controlmenu.h"

#include "stagemanager.h"
#include "game.h"
#include "startgame.h"

std::map<sf::Keyboard::Key, std::string> ControlMenu::m_keyStrings = {
    {sf::Keyboard::Num0, "0"},
    {sf::Keyboard::Num1, "1"},
    {sf::Keyboard::Num2, "2"},
    {sf::Keyboard::Num3, "3"},
    {sf::Keyboard::Num4, "4"},
    {sf::Keyboard::Num5, "5"},
    {sf::Keyboard::Num6, "6"},
    {sf::Keyboard::Num7, "7"},
    {sf::Keyboard::Num8, "8"},
    {sf::Keyboard::Num9, "9"},
    {sf::Keyboard::Numpad0, "0 - Pad"},
    {sf::Keyboard::Numpad1, "1 - Pad"},
    {sf::Keyboard::Numpad2, "2 - Pad"},
    {sf::Keyboard::Numpad3, "3 - Pad"},
    {sf::Keyboard::Numpad4, "4 - Pad"},
    {sf::Keyboard::Numpad5, "5 - Pad"},
    {sf::Keyboard::Numpad6, "6 - Pad"},
    {sf::Keyboard::Numpad7, "7 - Pad"},
    {sf::Keyboard::Numpad8, "8 - Pad"},
    {sf::Keyboard::Numpad9, "9 - Pad"},
    {sf::Keyboard::A, "A"},
    {sf::Keyboard::B, "B"},
    {sf::Keyboard::C, "C"},
    {sf::Keyboard::D, "D"},
    {sf::Keyboard::E, "E"},
    {sf::Keyboard::F, "F"},
    {sf::Keyboard::G, "G"},
    {sf::Keyboard::H, "H"},
    {sf::Keyboard::I, "I"},
    {sf::Keyboard::J, "J"},
    {sf::Keyboard::K, "K"},
    {sf::Keyboard::L, "L"},
    {sf::Keyboard::M, "M"},
    {sf::Keyboard::N, "N"},
    {sf::Keyboard::O, "O"},
    {sf::Keyboard::P, "P"},
    {sf::Keyboard::Q, "Q"},
    {sf::Keyboard::R, "R"},
    {sf::Keyboard::S, "S"},
    {sf::Keyboard::T, "T"},
    {sf::Keyboard::U, "U"},
    {sf::Keyboard::V, "V"},
    {sf::Keyboard::W, "W"},
    {sf::Keyboard::X, "X"},
    {sf::Keyboard::Y, "Y"},
    {sf::Keyboard::Z, "Z"},
    {sf::Keyboard::Right, "Flecha derecha"},
    {sf::Keyboard::Left, "Flecha izquierda"},
    {sf::Keyboard::Up, "Flecha arriba"},
    {sf::Keyboard::Down, "Flecha abajo"},
    {sf::Keyboard::LControl, "Control izquierda"},
    {sf::Keyboard::RControl, "Control derecha"},
    {sf::Keyboard::LShift, "Shift izquierda"},
    {sf::Keyboard::RShift, "Shift derecha"},
    {sf::Keyboard::Space, "Espacio"},

};

ControlMenu::ControlMenu(Context context, StageManager *parent):
    BaseContext(context),
    Stage(parent)
{
    m_colors.push_back(sf::Color::Red);
    m_colors.push_back(sf::Color::Green);
    m_colors.push_back(sf::Color(255, 255, 0));
    m_colors.push_back(sf::Color(255, 0, 255));
    m_colors.push_back(sf::Color(0, 255, 255));
    m_colors.push_back(sf::Color(0, 0, 0));

    m_canStart = false;

    sf::View view = m_context.window->getDefaultView();

    m_context.window->setView(view);
}

void ControlMenu::update(sf::Time eTime)
{
    if (m_keyBindings.size() > 1) m_canStart = true;
}

void ControlMenu::handleInput(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed) {
        sf::Keyboard::Key key = event.key.code;

        if (key == sf::Keyboard::Return) {
            if (m_keyBindings.size() >= 2) {
                m_parent->changeStage(new Game(m_context, m_parent, m_keyBindings));
                m_parent->pushStage(new StartGame(m_context, m_parent, 1));
            }
        }

        auto keyFound = m_keyStrings.find(key);

        if (keyFound != m_keyStrings.end() && m_keyBindings.size() < m_playerMax) {
            PlayerInfo info(m_colors.back(), key);

            auto infoFound = std::find(m_keyBindings.begin(), m_keyBindings.end(), info);

            if (infoFound == m_keyBindings.end()) {
                m_keyBindings.push_back(info);
                m_colors.pop_back();
            }
        }
    }
}

void ControlMenu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    size_t count = 0;

    for (auto& info : m_keyBindings) {
        sf::RectangleShape shape;

        shape.setFillColor(info.color);
        shape.setPosition(90.0f, count * 32.0f + 50.0f);
        shape.setSize({32.0f, 32.0f});
        target.draw(shape, states);

        sf::Text text;
        text.setFont(m_context.fonts->getResource("MAIN"));
        text.setPosition(130.0f, count * 32.0f + 50.0f);
        text.setString("Jugador " + std::to_string(count + 1) + " listo! " + m_keyStrings[info.key]);
        target.draw(text, states);

        count++;
    }

    for (size_t i = m_keyBindings.size(); i < m_playerMax; ++i) {
        sf::Text text;
        text.setFont(m_context.fonts->getResource("MAIN"));
        text.setPosition(90.0f, i * 32.0f + 50.0f);
        text.setScale(0.75f, 0.75f);
        text.setString("Presiona cualquier tecla para unirte");
        target.draw(text, states);
    }

    sf::Text startText;
    startText.setFont(m_context.fonts->getResource("MAIN"));
    startText.setPosition(90.0f, m_playerMax * 32.0f + 80.0f);
    startText.setScale(0.75f, 0.75f);

    if (m_canStart) startText.setString("Presiona Enter para comenzar!");
    else            startText.setString("Se necesitan al menos 2 jugadores para jugar");

    target.draw(startText, states);

    sf::Text ballText;
    ballText.setFont(m_context.fonts->getResource("MAIN"));
    ballText.setPosition(400.0f, 450.0f);
    ballText.setScale(0.5f, 0.5f);
    ballText.setString("Creado por Diego Ximenez 11-1-2015");

    target.draw(ballText, states);
}
