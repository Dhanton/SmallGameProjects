#ifndef STAGEMANAGER_H
#define STAGEMANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

#include "resloader.h"
#include "context.h"
#include "stage.h"

enum class Action_type
{
    POP,
    PUSH,
    CHANGE
};

struct Action
{
    Action(Action_type t, Stage* s = nullptr)
    {
        type = t;
        stage = s;
    }

    Action_type type;
    Stage*      stage;
};

class StageManager : public BaseContext
{
public:
    StageManager();

    void run();

    void pushStage(Stage* stage);
    void popStage();
    void changeStage(Stage* stage);

    void update(sf::Time eTime);
    void draw();
    void handleInput(const sf::Event& event);

    void exit();

    void setView(sf::View view);

private:
    sf::RenderWindow m_window;

    TextureLoader m_textures;
    FontLoader    m_fonts;

    std::vector<Stage::Ptr> m_stages;

    std::vector<Action> m_pendingChanges;
};

#endif // STAGEMANAGER_H
