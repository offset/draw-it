#include "tilemap.hpp"
#include "play.hpp"
#include <ctime>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>

bool tileMap::loadTileset(const std::string &tileset, sf::Vector2u tileSize, const std::vector<std::vector<int> > tiles, unsigned int width, unsigned int height)
{
    // load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
    {
        return false;
    }

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    // populate the vertex array with one quad per tile
    // => a quad is one tile
    for (unsigned int i = 0; i < width; ++i)
    {
        for (unsigned int j = 0; j < height; ++j)
        {
            // Which texture shall be drawn?
            int tileNumber = tiles[j][i];

            // find its position in the tileset texture
            // this tells us in which quad of the texturemap we have to look for the desired texture
            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            // define its 4 corners
            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }
    }
    Play::getInstance()->setMapSize(width * tileSize.x, height * tileSize.y);
    return true;
}

bool tileMap::load(sf::Vector2u tileSize, const std::vector<std::vector<int> > tiles, unsigned int width, unsigned int height)
{
    // load the tileset texture
//    if (!m_tileset.loadFromFile(tileset))
//    {
//        return false;
//    }

    boost::random::mt19937 gen(std::time(0));    
    
    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    // populate the vertex array with one quad per tile
    // => a quad is one tile
    for (unsigned int i = 0; i < width; ++i)
    {
        for (unsigned int j = 0; j < height; ++j)
        {
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
                sf::Color freeSpace(sf::Color::Blue);
                
                boost::random::uniform_int_distribution<> dist(1,4);
                
                if(dist(gen) == 1)
                {
                    freeSpace = sf::Color(0,0,225);
                }
                if(dist(gen) == 2)
                {
                    freeSpace = sf::Color(0,0,235);
                }
                if(dist(gen) == 3)
                {
                    freeSpace = sf::Color(0,0,245);
                }
                if(dist(gen) == 4)
                {
                    freeSpace = sf::Color(0,0,255);
                }
                
                quad[0].color = freeSpace;
                quad[1].color = freeSpace;
                quad[2].color = freeSpace;
                quad[3].color = freeSpace;
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
