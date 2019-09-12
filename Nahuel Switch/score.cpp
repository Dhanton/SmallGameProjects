#include "score.h"

#include "stagemanager.h"
#include "startgame.h"
#include "controlmenu.h"

Score::Score(Context context, StageManager *parent, sf::View view, float scrollingVel, size_t round, const std::vector<PlayerScore> &score, const std::vector<PlayerScore> &roundScore):
    BaseContext(context),
    Stage(parent)
{
    m_scrollingVel = scrollingVel;
    m_view = view;
    m_round = round;

    m_score = score;
    m_roundScore = roundScore;

    std::sort(m_score.begin(), m_score.end());
    std::sort(m_roundScore.begin(), m_roundScore.end());
}

void Score::update(sf::Time eTime)
{
    m_scrollingVel -= 100.0f * eTime.asSeconds();

    if (m_scrollingVel < 0.0f) m_scrollingVel = 0.0f;

    m_view.move(m_scrollingVel * eTime.asSeconds(), 0.0f);

    m_context.window->setView(m_view);
}

void Score::handleInput(const sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
        if (m_round == 6) {
            m_parent->changeStage(new ControlMenu(m_context, m_parent));
            return;
        }

        m_parent->popStage();
        m_parent->pushStage(new StartGame(m_context, m_parent, m_round + 1));
    }
}

void Score::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::View previousView = target.getView();
    sf::View view = target.getDefaultView();

    target.setView(view);

    sf::RectangleShape background;

    background.setFillColor(sf::Color(255.0f, 255.0f, 255.0f, 100.0f));
    background.setPosition(25.0f, 25.0f);
    background.setSize({view.getSize().x - 50.0f, view.getSize().y - 50.0f});

    target.draw(background, states);

    sf::Text text;

    text.setFont(m_context.fonts->getResource("MAIN"));
    text.setPosition(50.0f, 50.0f);
    text.setString("Ranking");
    target.draw(text, states);

    text.setPosition(300.0f, 50.0f);
    text.setString("Ronda " + std::to_string(m_round) + " de 6");
    target.draw(text, states);

    sf::RectangleShape playerShape;

    playerShape.setSize({32.0f, 20.0f});

    for (size_t i = 0; i < m_score.size(); ++i) {
        text.setPosition(50.0f, (i + 1) * 32.0f + 70.0f);
        text.setString(to_string(i + 1) + ".-" + to_string(m_score[i].score));
        target.draw(text, states);

        playerShape.setFillColor(m_score[i].info.color);
        playerShape.setPosition(190.0f, (i + 1) * 32.0f + 78.0f);
        target.draw(playerShape, states);
    }

    for (size_t i = 0; i < m_roundScore.size(); ++i) {
        text.setPosition(300.0f, (i + 1) * 32.0f + 70.0f);
        text.setString(to_string(i + 1) + ".-" + to_string(m_roundScore[i].score) + "%");
        target.draw(text, states);

        playerShape.setFillColor(m_score[i].info.color);
        playerShape.setPosition(465.0f, (i + 1) * 32.0f + 78.0f);
        target.draw(playerShape, states);
    }

    target.setView(previousView);
}

