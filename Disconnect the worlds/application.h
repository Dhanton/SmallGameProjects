#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics.hpp>

#include "player.h"
#include "resloader.h"
#include "game.h"
#include "menu.h"
#include "soundplayer.h"

class Application
{
public:
    Application();

    void run();

private:
    void update(sf::Time eTime);
    void draw();

    void handleInput(const sf::Event& event);

private:
    static constexpr unsigned int WIDTH = 1080;
    static constexpr unsigned int HEIGHT = 720;
    static constexpr float FACTOR = 4.0f;

    void cameraFollowPos(const sf::Vector2f& pos);
    void drawWinScreen();

    void updateWinScreen(sf::Time eTime);

private:
    sf::RenderWindow m_window;
    sf::View m_view;

    TextureLoader m_textureLoader;
    FontLoader    m_fontLoader;
    SoundPlayer<>   m_soundPlayer;

    Context m_context;

    Game::Ptr m_game;
    Menu::Ptr m_menu;

    bool m_isMenu;
    bool m_winScreen;

    bool m_focusing;
    sf::Text m_focusText;
    sf::Sprite m_textSprite;

    std::string m_toSay;
    std::string m_said;
    sf::Time    m_saidTime;

    sf::Sound m_talk;
};

#endif // APPLICATION_H
