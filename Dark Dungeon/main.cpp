#include "res_loader.hpp"
#include "player.hpp"

#include "entitymanager.hpp"
#include "map.hpp"
#include "light_manager.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

int main()
{
    srand(time(0));

    sf::RenderWindow window{{1080, 720}, "Prototype", sf::Style::Close | sf::Style::Titlebar};

    window.setFramerateLimit(120);

    TextureLoader texLoader;

    texLoader.loadResource("data/bullet.png", "BULLET");
    texLoader.loadResource("data/player.png", "PLAYER");
    texLoader.loadResource("data/missile.png", "MISSILE");
    texLoader.loadResource("data/monster.png", "MONSTER");
    texLoader.loadResource("data/worm.png", "WORM");
    texLoader.loadResource("data/tileset.png", "TILESET");
    texLoader.loadResource("data/normal_tileset.png", "NORMAL_TILESET");
    texLoader.loadResource("data/soul.png", "SOUL");

    Map map(&texLoader);

    //map.populate("data/map.png");
    //map.generateRandomly({30, 30});

    sf::Clock timer;
    sf::Time tickInter = sf::seconds(1.0f/30.0f);
    sf::Time dt;

    sf::View view = window.getDefaultView();
    //view.zoom(2);

    sf::Shader sceneShader;
    sceneShader.loadFromFile("scene_shader.frag", sf::Shader::Fragment);
    sf::RenderStates sceneState;
    sceneState.shader = &sceneShader;
    sceneState.blendMode = sf::BlendAdd;

    sf::RenderTexture normalMap;
    sf::RenderTexture geometry;
    normalMap.create(window.getSize().x, window.getSize().y);
    geometry.create(window.getSize().x, window.getSize().y);

    LightManager lights(window.getSize(), &normalMap);


    EntityManager eManager(&texLoader, &lights, &window, &geometry, &map);

    view.setCenter(eManager.getPlayer()->getCenter());

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Escape)
                window.close();

            if (event.type == sf::Event::Closed)
                window.close();

            eManager.getPlayer()->handleInput(event, window);
        }

        dt += timer.restart();

        while (dt >= tickInter) {
            eManager.update(dt);

            //Center camera around player
            Vector2 min = view.getSize()/2.f;
            Vector2 max = {map.totalMapSize().x - view.getSize().x/2.f, map.totalMapSize().y - view.getSize().y/2.f};
            Vector2 pCenter = eManager.getPlayer()->getCenter();
            Vector2 newCenter;

            newCenter.x = pCenter.x; //clamp(pCenter.x, max.x, min.x);
            newCenter.y = pCenter.y; //clamp(pCenter.y, max.y, min.y);

            view.setCenter(newCenter);

            dt -= tickInter;
        }

        //Lighting
        map.renderNormal(true);
        eManager.setNormalRender(true);
        normalMap.clear();
        normalMap.setView(view);
        normalMap.draw(map);
        normalMap.draw(eManager);
        normalMap.display();
        map.renderNormal(false);
        eManager.setNormalRender(false);

        lights.renderLights(&normalMap, &view);

        //Geometry
        geometry.clear();
        geometry.setView(view);
        geometry.draw(map);
        geometry.draw(eManager);
        geometry.display();

        //Render the scene
        sceneShader.setUniform("geometry", geometry.getTexture());
        sceneShader.setUniform("lightmap", lights.getLightMap()->getTexture());
        float ambDelta = 0.01;
        sceneShader.setUniform("environment", sf::Vector3f(ambDelta, ambDelta, ambDelta));

        window.clear();
        //window.setView(window.getDefaultView());

        sf::Sprite sprite;
        sprite.setTexture(lights.getLightMap()->getTexture());
        window.draw(sprite, sceneState);
        eManager.getPlayer()->renderBars(window);


        window.display();
    }

    return 0;
}

