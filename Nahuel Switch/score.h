#ifndef SCORE_H
#define SCORE_H

#include <sstream>
#include <iomanip>

#include "stage.h"
#include "context.h"
#include "player.h"

namespace
{
std::string to_string(float n, size_t precision = 3)
{
    std::ostringstream out;
    out << std::setprecision(precision) << n;
    return out.str();
}
}

class Score : public BaseContext, public Stage
{
public:
    Score(Context context, StageManager* parent, sf::View view, float scrollingVel, size_t round, const std::vector<PlayerScore>& score, const std::vector<PlayerScore> &roundScore);

    void update(sf::Time eTime);
    void handleInput(const sf::Event &event);

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    float m_scrollingVel;

    sf::View m_view;

    size_t m_round;

    std::vector<PlayerScore> m_score;
    std::vector<PlayerScore> m_roundScore;
};

#endif // SCORE_H
