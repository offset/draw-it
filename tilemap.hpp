#ifndef TILEMAP_H
#define TILEMAP_H
#include <vector>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

/*!
 * \brief The tileMap class
 *
 * This class is necessary to build our level. The code has been taken from
 * http://www.sfml-dev.org/tutorials/2.1/graphics-vertex-array.php (see 'Example: tile map')
 * and has been slightly modified.
 */
class tileMap : public sf::Drawable, public sf::Transformable
{
public:

    /*!
     * \brief Builds our level.
     *
     * It is necessary to supply a texturemap.
     *
     * \param tileset : The filename of the texturemap to be used.
     * \param tileSize : How big is one tile (texture)?
     * \param tiles : This controls which textures shall be loaded.
     * \param width : How many tiles shall be loaded per row?
     * \param height : How many tiles shall be loaded per column?
     * \return Success/Failure
     */
    
    bool loadTileset(const std::string& tileset, sf::Vector2u tileSize, const std::vector<std::vector<int> > tiles, unsigned int width, unsigned int height);
    bool load(sf::Vector2u tileSize, const std::vector<std::vector<int> > tiles, unsigned int width, unsigned int height);
    
private:

    /*!
     * \brief Draws the created vertex array (via load()).
     * \param target : The vertex array will be drawn here.
     * \param states : which texture shall be drawn
     */
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

#endif // TILEMAP_H
