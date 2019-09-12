#ifndef STARTGAME_H
#define STARTGAME_H

#include "stage.h"
#include "context.h"

class StartGame : public BaseContext, public Stage
{
public:
    StartGame(Context context, StageManager* parent, size_t rounds);

    void update(sf::Time eTime);
    void handleInput(const sf::Event &event);

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    size_t m_rounds;

    sf::Time m_toStart;
};

#endif // STARTGAME_H
