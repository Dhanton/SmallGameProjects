#include "minemanager.h"

MineManager::MineManager(Context &context):
    m_texture(context.textureLoader->getResource(TextureID::OTHERS))
{
}

void MineManager::update(sf::Time eTime, ExplosionManager& explosionMan)
{
    for (auto& mine : m_mines)
    {
        mine->update(eTime);
    }

    if (m_mines.size() >= 10)
    {
        explosionMan.addExplosion({m_mines.back()->getPosition().x - 6.0f, m_mines.back()->getPosition().y - 6.0f});
        m_mines.pop_back();
    }
}

void MineManager::addMine(const sf::Vector2f position)
{
    std::unique_ptr<AnimationManager> mine(new AnimationManager(m_texture, {16, 16}));

    mine->setPosition(position);
    mine->addAnimation("MAIN", 12, 2, sf::seconds(0.5f), true);
    mine->playAnimation("MAIN");

    m_mines.push_front(std::move(mine));
}

bool MineManager::checkCollision(sf::FloatRect rect, ExplosionManager& explosionMan)
{
    for (auto& mine : m_mines)
    {
        sf::FloatRect mineRect = mine->getSpriteBounds();

        mineRect.width = 22;
        mineRect.height = 20;

        if (rect.intersects(mineRect))
        {
            explosionMan.addExplosion({mine->getPosition().x - 6.0f, mine->getPosition().y - 6.0f});

            m_mines.erase(std::remove_if(m_mines.begin(), m_mines.end(),
                                         [&] (std::unique_ptr<AnimationManager> const &m){return mine == m;}),
                          m_mines.end());

            return true;
        }
    }

    return false;
}

void MineManager::clear()
{
    m_mines.clear();
}

bool MineManager::isEmtpy() const
{
    return m_mines.empty();
}

void MineManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto& mine : m_mines)
    {
        target.draw(*mine, states);
    }
}
