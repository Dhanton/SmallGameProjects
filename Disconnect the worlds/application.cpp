#include "application.h"

Application::Application()
{
    m_window.create({WIDTH, HEIGHT}, "Disconnect the worlds", sf::Style::Close | sf::Style::Titlebar);
    m_window.setFramerateLimit(120);

    m_view = m_window.getDefaultView();
    m_view.zoom(1.0f / FACTOR);
    m_view.setCenter(m_view.getSize() / 2.0f);

    m_textureLoader.loadResource("gfx/tileset.png", TextureID::GOOD_WORLD);
    m_textureLoader.loadResource("gfx/others.png", TextureID::OTHERS);
    m_textureLoader.loadResource("gfx/hell_tileset.png", TextureID::BAD_WORLD);
    m_textureLoader.loadResource("gfx/player.png", TextureID::PLAYER);

    m_fontLoader.loadResource("fonts/menu.ttf", FontID::MENU);
    m_fontLoader.loadResource("fonts/main_font.ttf", FontID::MAIN);

    m_soundPlayer.loadSound(SoundID::EXPLOSION, "sfx/explosion.wav");
    m_soundPlayer.loadSound(SoundID::BULLET, "sfx/bullet.wav");
    m_soundPlayer.loadSound(SoundID::PICK_UP, "sfx/pick_up.wav");
    m_soundPlayer.loadSound(SoundID::PLAYER_HURT, "sfx/player_hurt.wav");
    m_soundPlayer.loadSound(SoundID::TALKING, "sfx/talk.wav");
    m_soundPlayer.loadSound(SoundID::PORTAL, "sfx/portal.wav");
    m_soundPlayer.loadSound(SoundID::DOOR, "sfx/door.wav");

    m_context.textureLoader = &m_textureLoader;
    m_context.window = &m_window;
    m_context.fontLoader = &m_fontLoader;
    m_context.soundPlayer = &m_soundPlayer;

    m_game = Game::Ptr(new Game(m_context));
    m_menu = Menu::Ptr(new Menu(m_context));

    m_isMenu = true;

    m_focusing = true;

    m_textSprite.setTexture(m_textureLoader.getResource(TextureID::OTHERS));
    m_textSprite.setTextureRect({6 * 16, 2 * 16, 4 * 16, 2 * 16});
    m_textSprite.scale(2.0f, 2.0f);
    m_textSprite.setPosition(350.0f, 350.0f);

    m_focusText.setString("Click to focus!");
    m_focusText.setFont(m_fontLoader.getResource(FontID::MAIN));
    m_focusText.setCharacterSize(10.0f);
    m_focusText.setPosition(m_textSprite.getPosition().x + 17.0f, m_textSprite.getPosition().y + 26.0f);

    m_winScreen = false;

    m_toSay = "You defeated the monster and closed the portal!\n\nYou have  brought peace and silence into the world...\n\nWhat can I do for you??\n\n\n\n\nJust kidding, I'm a menu :D\n\n\nBut you can press Action and start again if you want...\n\n\n\n\nDO IT!!!";

    m_talk.setBuffer(m_soundPlayer.getSoundBuffer(SoundID::TALKING));
    m_talk.setVolume(30.0f);
}

void Application::run()
{
    sf::Clock clock;

    while (m_window.isOpen())
    {
        sf::Event event;

        while (m_window.pollEvent(event))
        {
            handleInput(event);
        }

        update(clock.restart());

        draw();
    }
}

void Application::update(sf::Time eTime)
{
    if (!m_focusing) return;

    if (m_winScreen)
        updateWinScreen(eTime);

    else if (m_isMenu)
    {
        m_menu->update(eTime);

        if (m_menu->isFinished()) m_isMenu = false;
    }
    else
    {
        m_game->update(eTime);
        cameraFollowPos(m_game->getPlayerPos());

        if (m_game->isFinished()) m_winScreen = true;
    }

    m_soundPlayer.removeStoppedSounds();
}

void Application::draw()
{
    m_window.clear();
    m_window.setView(m_view);

    if (m_winScreen)
        drawWinScreen();

    else if (m_isMenu)
        m_window.draw(*m_menu);

    else
        m_window.draw(*m_game);

    if (!m_focusing)
    {
        sf::View view = m_window.getDefaultView();
        view.zoom(1.0f / 3.0f);
        view.setCenter(m_textSprite.getPosition().x + m_textSprite.getGlobalBounds().width / 2.0f,
                       m_textSprite.getPosition().y + m_textSprite.getGlobalBounds().height / 2.0f);

        m_window.setView(view);

        m_window.draw(m_textSprite);
        m_window.draw(m_focusText);
    }

    m_window.display();
}

void Application::handleInput(const sf::Event &event)
{
    if (!m_focusing)
    {
        if (event.type == sf::Event::GainedFocus)
            m_focusing = true;
        return;
    }
    else
    {
        if (event.type == sf::Event::LostFocus)
        {
            m_focusing = false;
            return;
        }
    }

    if (event.type == sf::Event::Closed)
        m_window.close();
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape)
        m_window.close();

    if (m_winScreen)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
        {
            m_isMenu = true;
            m_winScreen = false;
            m_game = Game::Ptr(new Game(m_context));
            return;
        }
    }

    else if (m_isMenu)
        m_menu->handleInput(event);
    else
        m_game->handleInput(event);
}

void Application::cameraFollowPos(const sf::Vector2f &pos)
{
    sf::Vector2f cameraPos = pos;

    if (pos.x <= m_view.getSize().x / 2.0f) cameraPos.x = m_view.getSize().x / 2.0f;
    if (pos.y <= m_view.getSize().y / 2.0f) cameraPos.y = m_view.getSize().y / 2.0f;

    if (pos.x >= 49.0f * 16.0f - m_view.getSize().x / 2.0f) cameraPos.x = 49.0f * 16.0f - m_view.getSize().x / 2.0f;
    if (pos.y >= 49.0f * 16.0f - m_view.getSize().y / 2.0f) cameraPos.y = 49.0f * 16.0f - m_view.getSize().y / 2.0f;

    m_view.setCenter(cameraPos);
}

void Application::drawWinScreen()
{
    m_window.setView(m_window.getDefaultView());

    sf::Text text;

    text.setFont(m_context.fontLoader->getResource(FontID::MAIN));
    text.setString(m_said);
    text.setPosition(10.0f, 10.0f);

    m_window.draw(text);
}

void Application::updateWinScreen(sf::Time eTime)
{
    /* Thanks Xay for this awesome code :D */

    if (m_toSay.size() == m_said.size()) return;

    static const std::string abc = ".,abcdefghijklmnopqrstuvwxyz                                       ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890?!";

    m_saidTime += eTime;

    sf::Time toNext = sf::milliseconds(55);

    char currentChar = m_toSay[m_said.size()];

    if (currentChar == '\n' || currentChar == '.' || currentChar == '!' || currentChar == '?' || currentChar == ',') toNext = sf::milliseconds(200);

    if (m_saidTime >= toNext)
    {
        m_saidTime -= toNext;
        m_said.push_back(m_toSay[m_said.size()]);

        float factor = abc.find(currentChar);

        m_talk.setPitch(factor / abc.length() * 0.8f + 0.8f);
        m_talk.play();
    }
}
