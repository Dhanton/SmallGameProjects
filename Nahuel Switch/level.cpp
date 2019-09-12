#include "level.h"

#include "mathhelper.h"

Level::Level(Context context, const std::vector<PlayerInfo> &playersInfo):
    BaseContext(context)
{
    m_view = context.window->getView();

    m_scrollingVel = 375.0f;
    m_finalPos = 0.0f;
    m_initialPos = m_view.getCenter();

    m_tileSize = 0;

    for (auto& info : playersInfo)
        m_players.push_back(Player::Ptr(new Player({0.0f, 0.0f}, info, m_players)));
}

void Level::update(sf::Time eTime)
{
    if (m_scrollingVel < 450.0f) m_scrollingVel += 20.0f * eTime.asSeconds();

    m_view.move(m_scrollingVel * eTime.asSeconds(), 0.0f);

    for (auto& player : m_players) {
        if (!player->getRect().intersects(Helper::viewToRect(m_view))) {
            player->die();
            continue;
        }

        for (auto chunk : m_chunks) {
            if (player->getParentChunk() != chunk && chunk->getRect().intersects(player->getRect())) {
                player->setParentChunk(chunk);
                break;
            }
        }
    }

    for (auto& player : m_players) {
        player->update((m_scrollingVel + (player->getPosition().x < m_view.getCenter().x ? 70.0f : 0.0f)), eTime);
    }
}

void Level::handleInput(const sf::Event &event)
{
    for (auto& player : m_players) player->handleInput(event);
}

void Level::loadLevel(const std::string& filename)
{
    std::string errorStr = "XML Parsing problem in file " + filename;

    rapidxml::file<> file(filename.c_str());

    rapidxml::xml_document<> doc;
    doc.parse<0>(file.data());

    rapidxml::xml_node<>* root = doc.first_node("map");

    m_tileSize = std::stoi(root->first_attribute("tilewidth")->value());

    if (m_tileSize != std::stoi(root->first_attribute("tileheight")->value())) throw std::runtime_error(errorStr + " - Tile width must be equal to tile height.");

    for (rapidxml::xml_node<>* child = root->first_node(); child != nullptr; child = child->next_sibling()) {

        if (child->name() == std::string("tileset")) {
            if (std::stoi(child->first_attribute("tilewidth")->value()) != m_tileSize || std::stoi(child->first_attribute("tileheight")->value()) != m_tileSize)
                throw std::runtime_error(errorStr + " - Map tile size must be equal to image tile size.");

            m_textureID = child->first_attribute("name")->value();
        }

        if (child->name() == std::string("layer")) {
            loadLayer(child, filename);
        }
    }
}

void Level::buildLevel(size_t chunkNumber)
{
    assert(chunkNumber <= m_patterns.size());
    
    m_chunks.clear();

    std::vector<size_t> indices(m_patterns.size() - 2);

    for (size_t i = 0; i < indices.size(); ++i) indices[i] = i + 1;

    std::random_shuffle(indices.begin(), indices.end(), [] (size_t i) -> size_t {return std::rand() % i;});

    m_chunks.push_back(&m_patterns.front());

    for (size_t i = 0; i < chunkNumber - 2; ++i) {
        m_chunks.push_back(&m_patterns.at(indices[i]));

        /* Set position to previousPos.x + previousWidth */
        m_chunks[i + 1]->setPosition(m_chunks[i]->getPosition().x + m_chunks[i]->getRect().width, 0);
    }

    m_chunks.push_back(&m_patterns.back());
    m_chunks.back()->setPosition(m_chunks.at(m_chunks.size() - 2)->getPosition().x + m_chunks.at(m_chunks.size() - 2)->getRect().width, 0);
    m_finalPos = m_chunks.back()->getPosition().x + m_chunks.back()->getRect().width - 2 * m_tileSize;

    respawnPlayers();

    m_view.setCenter(m_initialPos);
}

void Level::respawnPlayers()
{
    float xPos = 200.0f;
    float playerHeight = m_players.front()->getRect().height;

    std::random_shuffle(m_players.begin(), m_players.end(), [] (int p) {return std::rand() % p;});

    m_players.at(0)->respawn({xPos, 9.0f * m_tileSize - playerHeight});
    m_players.at(1)->respawn({xPos, 9.0f * m_tileSize - 2 * playerHeight});

    if (m_players.size() > 2) m_players.at(2)->respawn({xPos, 5.0f * m_tileSize - playerHeight});
    if (m_players.size() > 3) m_players.at(3)->respawn({xPos, 5.0f * m_tileSize - 2 * playerHeight});

    if (m_players.size() > 4) m_players.at(4)->respawn({xPos, 13.0f * m_tileSize - playerHeight});
    if (m_players.size() > 5) m_players.at(5)->respawn({xPos, 13.0f * m_tileSize - 2 * playerHeight});
}

std::vector<PlayerScore> Level::getPlayerScore(float ratio) const
{
    std::vector<PlayerScore> playerScore;

    for (auto& player : m_players) {
        float score = std::round((player->getPosition().x / m_finalPos) * ratio * 100.0f)/100.0f;
        playerScore.push_back(PlayerScore(player->getInfo(), score));
    }

    return playerScore;
}

size_t Level::getPlayerNumber() const
{
    return m_players.size();
}

bool Level::isFinished() const
{
    if (m_view.getCenter().x >= m_finalPos +  2 * m_tileSize + m_view.getSize().x / 2.0f) return true;

    for (const auto& player : m_players) {
        if (!player->isDead()) return false;
    }

    return true;
}

float Level::getScrollingVel() const
{
    return m_scrollingVel;
}

sf::View Level::getView() const
{
    return m_view;
}

void Level::loadLayer(rapidxml::xml_node<> *parent, const std::string& filename)
{
    std::string errorStr = "XML Parsing problem in file " + filename;

    size_t width = std::stoi(parent->first_attribute("width")->value());
    size_t height = std::stoi(parent->first_attribute("height")->value());

    TileMap pattern(width, height, m_tileSize, &m_context.textures->getResource(m_textureID));

    rapidxml::xml_node<>* dataNode = parent->first_node("data");

    std::string encoding = dataNode->first_attribute("encoding")->value();
    std::string compression = dataNode->first_attribute("compression")->value();

    if (encoding != "base64") throw std::runtime_error(errorStr + " - Wrong enconding, it must be base64.");
    if (compression != "zlib") throw std::runtime_error(errorStr + " - Wrong compression, it must be zlib.");

    std::string encodedValue = dataNode->value();

    /* Remove spaces and returns */
    std::remove_if(encodedValue.begin(), encodedValue.end(), [](char c){return c==' '||c=='\n';});

    std::string decodedMapString = base64::decode(encodedValue);

    std::vector<int> ids(width * height);

    uLongf idsSize = width * height * sizeof(int);

    uncompress((Bytef*) &ids[0], &idsSize, (const Bytef*) decodedMapString.c_str(), decodedMapString.size());

    for (size_t j = 0; j < height; ++j)
        for (size_t i = 0; i < width; ++i) {
            pattern.setTile(i, j, ids[j * width + i]);
        }

    m_patterns.push_back(pattern);
}

void Level::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::FloatRect cameraRect = Helper::viewToRect(target.getView());

    for (auto chunk : m_chunks) {
        if (cameraRect.intersects(chunk->getRect())) target.draw(*chunk, states);
    }

    for (auto& player : m_players) {
        if (cameraRect.intersects(player->getRect())) target.draw(*player, states);
    }
}
