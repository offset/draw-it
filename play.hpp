#ifndef PLAY_H
#define PLAY_H
#include <string>
#include <vector>
#include "linefinder.hpp"
#include "game.hpp"
#include "tilemap.hpp"

/*!
 * \brief The Play class
 *
 * This class acts as a wrapper for the detection and game functionalities of the program. It calls each in a seperate
 * function. The class is implemented with the singleton pattern, meaning that the classes functions must be called via 
 * Play::getInstance()->function.
 */

class Play
{
public:
    ~Play();
    
private:
    static Play* singleton;
    LineFinder* finder = nullptr;
    Play() : minLength(100.f), maxGap(40.f), minVote(80), skelThreshold(50), 
        cannyThreshold1(40), cannyThreshold2(300), cannyApertureSize(3),
        l2Gradient(true)
    {
        finder = new LineFinder;
    }
    Play(const Play& copy) {}
    Play& operator=(const Play& copy) {return *this;}
    
public:
    /*!
     * \brief Used to access Play class which has a static constructor.
     * \return pointer to a Play instance
     */
    static Play* getInstance();
    
    /*!
     * \brief Access the LineFinder object.
     * \return A pointer to the LineFinder object.
     */
    LineFinder* getFinder();
    
    static void destroy();
    
    /*!
     * \brief Calls all neccessary functions to detect the lines in an image and saves them in form of a two-dimensional vector which can be accessed via getLevelMap().
     * \param fileNameLevel[in]: how should the generated level file be named
     * \param minLength[in]: minimal length for LineFinder class
     * \param maxGap[in]: minimal gap for LineFinder class
     * \param minVote[in]: minimal amount of votes for LineFinder class
     * \param skelThreshold: minimal threshold for the generation of the skeleton of the image
     * \param cannyThreshold1[in]: minimal threshold for contour detection with the canny-edge filter.
     * \param cannyThreshold2[in]: maximum threshold for contour detection with the canny-edge filter.
     * \param cannyApertureSize[in]: Aperture Size for the canny-edge detector. If set higher it should remove some noise, but due to skeleton this is unneccessary.
     * \param l2Gradient[in]: If set to true, the calculation is more demanding, but also yields better results.
     * \return error/success code
     */
    int detect(float minLength = 100.f, 
                       float maxGap = 40.f, 
                       int minVote = 80,
                       int skelThreshold = 50,
                       int cannyThreshold1 = 40,
                       int cannyThreshold2 = 300,
                       int cannyApertureSize = 3,
                       bool l2Gradient = true
                       );
    
    /*!
     * \brief Builds a level based on a text file and instantiates a game object and calls it's run method.
     * \return Error/Success Code
     */
    int play();
    
    /*!
     * \brief Change the private Level Map.
     */
    void setLevelMap(std::vector<std::vector<int> > lm);
    
    /*!
     * \brief Access the Level Map.
     * \return The level Map
     */
    std::vector<std::vector<int> > & getLevelMap();
    
    /*!
     * \brief Access the tile map
     * \return The tile map.
     */
    tileMap & getTileMap();
    
    sf::Vector2u getMapSize();
    
    void setMapSize(int width, int height);
    
    /*!
     * \brief Access the physics map.
     * \return The physics map.
     */
    std::vector<std::vector<int> > & getPhysicsMap();
    
    /*!
     * \brief Set the vector to use for the physics map.
     */
    void setPhysicsMap(std::vector<std::vector<int> > newPhMap);
    
    float getMinLength();
    void setMinLength(float mLength);
    
    float getMaxGap();
    void setMaxGap(float maxGap);
    
    int getMinVote();
    void setMinVote(int mVote);
    
    int getSkelThreshold();
    void setSkelThreshold(int skThreshold);
    
    int getCannyThreshold1();
    void setCannyThreshold1(int cThreshold1);
    
    int getCannyThreshold2();
    void setCannyThreshold2(int cThreshold2);
    
    int getCannyApertureSize();
    void setCannyApertureSize(int cASize);
    
    bool getL2Gradient();
    void setL2Gradient(bool l2Grad);
    
private:
    
    /*!
     * \brief Builds the level as a tileMap consisting of vertex arrays. See more at class tileMap.
     *
     * \return Error/Success Code
     */
    int buildLevel();
    // contains the data to build the level
    std::vector<std::vector<int> > levelMap;
    // Contains the built level.
    tileMap m_TileMap;
    // Tells the game where there are solid objects.
    std::vector<std::vector<int> > physicsMap;
    sf::Vector2u mapSize;
    
    float minLength;
    float maxGap; 
    int minVote;
    int skelThreshold;
    int cannyThreshold1;
    int cannyThreshold2;
    int cannyApertureSize;
    bool l2Gradient;
};

#endif // PLAY_H
