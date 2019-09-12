#ifndef GAME_H
#define GAME_H

#include "stage.h"
#include "level.h"
#include "context.h"

class Game : public Stage, public BaseContext
{
public:
    Game(Context context, StageManager *parent, const std::vector<PlayerInfo> &playersInfo);

    void update(sf::Time eTime);
    void handleInput(const sf::Event &event);

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void upgradeScore();

private:
    static constexpr size_t m_chunksPerLevel = 4;

    size_t m_rounds;

    Level m_level;

    std::vector<PlayerScore> m_score;
};

#endif // GAME_H
