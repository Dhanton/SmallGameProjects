#include "game.h"

#include "stagemanager.h"

#include "controlmenu.h"
#include "score.h"

Game::Game(Context context, StageManager *parent, const std::vector<PlayerInfo> &playersInfo):
    Stage(parent),
    BaseContext(context),
    m_level(context, playersInfo)
{
    std::string filename = "assets/patterns.tmx";

    m_level.loadLevel(filename);
    m_level.buildLevel(m_chunksPerLevel);

    m_rounds = 1;
}

void Game::update(sf::Time eTime)
{
    if (m_level.isFinished()) {
        upgradeScore();
        m_parent->pushStage(new Score(m_context, m_parent, m_level.getView(), m_level.getScrollingVel(), m_rounds, m_score, m_level.getPlayerScore(100)));
        m_level.buildLevel(m_chunksPerLevel);
        m_rounds++;
        return;
    }

    m_level.update(eTime);

    m_parent->setView(m_level.getView());
}

void Game::handleInput(const sf::Event &event)
{
    m_level.handleInput(event);
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_level, states);
}

void Game::upgradeScore()
{
    std::vector<PlayerScore> newScore = m_level.getPlayerScore();

    if (m_score.empty()) {
        for (size_t i = 0; i < newScore.size(); ++i) m_score.push_back(newScore[i]);
        return;
    }

    assert(newScore.size() == m_score.size());

    for (auto newPlayerScore : newScore) {
        for (auto& oldScore : m_score) {
            if (newPlayerScore.info == oldScore.info) oldScore.score += newPlayerScore.score;
        }
    }
}
