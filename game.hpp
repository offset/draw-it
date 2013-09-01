#ifndef GAME_H
#define GAME_H
#include <string>
#include "linefinder.hpp"
#include "entities.h"

// error/success codes
#define SUCCESS 0
#define READ_ERROR -1

/*!
 * \brief The Game class
 *
 * This class acts as a wrapper for the detection and game functionalities of the program. It calls each in a seperate
 * function. The class is implemented with the singleton pattern, meaning that the classes functions must be called via 
 * Game::getInstance()->function.
 */

class Game
{
public:
    ~Game();
    
private:
    static Game* singleton;
    LineFinder* finder = nullptr;
    Game()
    {
        finder = new LineFinder;
    }
    Game(const Game& copy) {}
    Game& operator=(const Game& copy) {return *this;}
    
public:
    /*!
     * \brief Used to access Game class which has a static constructor.
     * \return pointer to a Game instance
     */
    static Game* getInstance();
    
    static void destroy();
    
    /*!
     * \brief Calls all neccessary functions to detect the lines in an image and writes them to a text file.
     * \param fileNameImage[in]: which image shall be used
     * \param fileNameLevel[in]: how should the generated level file be named
     * \param minLength[in]: minimal length for LineFinder class
     * \param minGap[in]: minimal gap for LineFinder class
     * \param minVote[in]: minimal amount of votes for LineFinder class
     * \param skelThreshold: minimal threshold for the generation of the skeleton of the image
     * \param cannyThreshold1[in]: minimal threshold for contour detection with the canny-edge filter.
     * \param cannyThreshold2[in]: maximum threshold for contour detection with the canny-edge filter.
     * \param cannyApertureSize[in]: Aperture Size for the canny-edge detector. If set higher it should remove some noise, but due to skeleton this is unneccessary.
     * \param l2Gradient[in]: If set to true, the calculation is more demanding, but also yields better results.
     * \return Path to the created level text file.
     */
    std::string detect(std::string fileNameImage = "../level.png",
                       std::string fileNameLevel = "../level.txt",
                       float minLength = 100.f, 
                       float minGap = 40.f, 
                       int minVote = 80,
                       int skelThreshold = 50,
                       int cannyThreshold1 = 40,
                       int cannyThreshold2 = 300,
                       int cannyApertureSize = 3,
                       bool l2Gradient = true
                       );
    
    /*!
     * \brief Implements functionalities for a simple jump'n'run game.
     * \param fileNameLevel[in]: Specifies which level file to use.
     * \return Error/Success Code
     */
    int play(std::string fileNameLevel = "../level.txt");
};

#endif // Game_H
