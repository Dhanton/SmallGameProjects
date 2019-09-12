#include "startgame.h"

#include "stagemanager.h"

StartGame::StartGame(Context context, StageManager *parent, size_t rounds):
    BaseContext(context),
    Stage(parent)
{
    m_rounds = rounds;

    sf::View view = m_context.window->getDefaultView();

    m_context.window->setView(view);
}

void StartGame::update(sf::Time eTime)
{
    m_toStart += eTime;
}

void StartGame::handleInput(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
        m_parent->popStage();
    }
}

void StartGame::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::View previousView = target.getView();
    sf::View view = target.getDefaultView();

    target.setView(view);

    sf::RectangleShape shape;

    shape.setFillColor(sf::Color(255, 255, 255, 100));
    shape.setPosition(25.0f, 65.0f);
    shape.setSize({500.0f, 70.0f});

    target.draw(shape, states);

    sf::Text text;

    text.setFont(m_context.fonts->getResource("MAIN"));
    text.setString("Ronda " + std::to_string(m_rounds) + " de 6");
    text.setPosition(35.0f, 75.0f);
    text.setScale(0.75f, 0.75f);

    target.draw(text, states);

    text.setString("Presiona Enter para empezar a jugar");
    text.setPosition(35.0f, 100.0f);

    target.draw(text, states);

    target.setView(previousView);
}
