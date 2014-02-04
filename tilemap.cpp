#include "tilemap.hpp"
#include "play.hpp"

//bool tileMap::load(const std::string &tileset, sf::Vector2u tileSize, const std::vector<std::vector<int> > tiles, unsigned int width, unsigned int height)
bool tileMap::load(sf::Vector2u tileSize, const std::vector<std::vector<int> > tiles, unsigned int width, unsigned int height)
{
    // load the tileset texture
//    if (!m_tileset.loadFromFile(tileset))
//    {
//        return false;
//    }

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);
    int height2 = 0;

    // populate the vertex array with one quad per tile
    // => a quad is one tile
    for (unsigned int i = 0; i < width; ++i)
    {
        for (unsigned int j = 0; j < height; ++j)
        {
            ++height2;
            // Which texture shall be drawn?
            //int tileNumber = tiles[j][i];

            // find its position in the tileset texture
            // this tells us in which quad of the texturemap we have to look for the desired texture
            //int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            //int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            // define its 4 corners
            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            // set the color of the vertex array
            if (tiles[j][i] == 0)
            {
                quad[0].color = sf::Color::Green;
                quad[1].color = sf::Color::Green;
                quad[2].color = sf::Color::Green;
                quad[3].color = sf::Color::Green;
            } else if(tiles[j][i] == 1)
            {
                quad[0].color = sf::Color::White;
                quad[1].color = sf::Color::White;
                quad[2].color = sf::Color::White;
                quad[3].color = sf::Color::White;
            }
        }
    }
    Play::getInstance()->setMapSize(width * tileSize.x, height * tileSize.y);
    return true;
}

void tileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = &m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}
