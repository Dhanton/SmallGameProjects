#include "stagemanager.h"

#include "game.h"
#include "controlmenu.h"

StageManager::StageManager():
    BaseContext(Context(&m_textures, &m_fonts, &m_window))
{
    m_window.create({720, 480}, "Nahuel Switch - Beta", sf::Style::Close  | sf::Style::Titlebar);
    m_window.setFramerateLimit(60);

    rapidxml::file<> file("assets/patterns.tmx");

    rapidxml::xml_document<> doc;
    doc.parse<0>(file.data());

    rapidxml::xml_node<>* tilesetNode = doc.first_node("map")->first_node("tileset");

    std::string textureID = tilesetNode->first_attribute("name")->value();
    std::string path      = tilesetNode->first_node("image")->first_attribute("source")->value();

    m_textures.loadResource("assets/" + path, textureID);
    m_fonts.loadResource("assets/main_font.ttf", "MAIN");

    pushStage(new ControlMenu(m_context, this));
}

void StageManager::run()
{
    sf::Clock clock;

    while (m_window.isOpen()) {
        sf::Event event;

        while (m_window.pollEvent(event)) handleInput(event);

        update(clock.restart());
        draw();
    }
}

void StageManager::pushStage(Stage *stage)
{
    m_pendingChanges.push_back(Action(Action_type::PUSH, stage));
}

void StageManager::popStage()
{
    m_pendingChanges.push_back(Action(Action_type::POP));
}

void StageManager::changeStage(Stage *stage)
{
    m_pendingChanges.push_back(Action(Action_type::CHANGE, stage));
}

void StageManager::update(sf::Time eTime)
{
    if (m_pendingChanges.size() > 0) {
        for (Action action : m_pendingChanges) {
            switch (action.type) {
            case Action_type::CHANGE:
                m_stages.clear();
                m_stages.push_back(Stage::Ptr(action.stage));
                break;
            case Action_type::POP:
                m_stages.pop_back();
                break;
            case Action_type::PUSH:
                m_stages.push_back(Stage::Ptr(action.stage));
                break;
            default:
                break;
            }
        }

        m_pendingChanges.clear();

        return;
    }

    if (m_stages.size() > 0) m_stages.back()->update(eTime);
}

void StageManager::draw()
{
    m_window.clear(sf::Color(75, 75, 229));

    if (m_stages.size() > 0) for (auto& stage : m_stages) m_window.draw(*stage);

    m_window.display();
}

void StageManager::handleInput(const sf::Event &event)
{
    if (event.type == sf::Event::Closed) m_window.close();
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) m_window.close();

    if (m_stages.size() > 0) m_stages.back()->handleInput(event);
}

void StageManager::exit()
{
    m_window.close();
}

void StageManager::setView(sf::View view)
{
    m_window.setView(view);
}
