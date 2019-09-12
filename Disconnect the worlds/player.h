#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <array>

#include "entity.h"
#include "obstaclemanager.h"
#include "direction.h"
#include "bulletmanager.h"
#include "card.h"
#include "animationmanager.h"

class Player : public Entity
{
public:
    typedef std::unique_ptr<Player> Ptr;

public:
    Player(Context& context);

    void update(sf::Time eTime, const ObstacleManager &obsMan,
                const TileMap& map, BulletManager &bMan);

    void update(sf::Time eTime, BulletManager &bMan);

    void drawHud(sf::RenderTarget &target, sf::RenderStates states) const;

    void handleInput(const sf::Event& event);

    void                    pickUpCard(CardType type, bool gold = false);
    void                    destroyCard(CardType type);
    bool                    hasCard(CardType type, bool gold = false) const;
    std::array<CardType, 3> getCurrentCards(bool gold = false) const;

    bool canOpenGoldDoor() const;
    bool isDead() const;

    void respawn(bool destroyCards = true);

    void takeDamage();

    virtual sf::FloatRect getGlobalBounds();

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void updateAnimation(sf::Time eTime);

private:
    SoundPlayer<>* m_soundPlayer;

    AnimationManager m_animation;

    Direction m_looking;

    const float maxVel;

    bool           m_shoot;
    sf::Time       m_shootTime;
    const sf::Time m_shootInterval;

    bool m_dead;
    unsigned int m_lifes;

    sf::Time m_invTime;
    bool m_isInv;

    std::array<CardType, 3> m_currentCards;
    std::array<CardType, 3> m_goldenCards;

    mutable AnimationManager m_hud;
};

#endif // PLAYER_H
