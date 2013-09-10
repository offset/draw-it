#ifndef PLAY_H
#define PLAY_H
#include <string>
#include "linefinder.hpp"
#include "game.hpp"

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
    Play()
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
    std::vector<std::vector<int> > detect(std::string fileNameImage = "../level.png",
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
     * \brief Builds a level based on a text file and instantiates a game object and calls it's run method.
     * \param vector which contains the lines
     * \return Error/Success Code
     */
    int play(std::vector<std::vector<int> > levelFile);
    
    /*!
     * \brief Builds a level based on the contents of the text file.
     * \param fileNameLevel[in]: Specifies which file to use.
     * \return 
     */
    
private:
    int buildLevel(std::vector<std::vector<int> > levelFile);
};

#endif // PLAY_H
