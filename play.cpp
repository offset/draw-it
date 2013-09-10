#include "play.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

Play* Play::singleton = 0;

Play::~Play()
{
    delete finder;
}

Play* Play::getInstance()
{
    if (singleton == 0)
    {
        singleton = new Play();
    }
    return singleton;
}

void Play::destroy()
{
    if (singleton != 0)
    {
        delete singleton;
        singleton = 0;
    }
}

std::vector<std::vector<int> > Play::detect(std::string fileNameImage,
                   float minLength , 
                   float minGap, 
                   int minVote,
                   int skelThreshold,
                   int cannyThreshold1,
                   int cannyThreshold2,
                   int cannyApertureSize,
                   bool l2Gradient
                   )
{
    LineFinder finder;
    finder.setLineLengthAndGap(minLength, minGap);
    finder.setMinVote(minVote);
    
    cv::Mat image = cv::imread(fileNameImage);
    if(!image.data)
    {
        std::cout << "File could not be loaded..." <<
                "Exiting." << std::endl;
        exit;
    }
    
    // creating a skeleton
    image = finder.createSkeleton(image, skelThreshold);
    finder.saveToDisk(image);
    
    // detecting the contours
    cv::Mat contours;
    cv::Canny(image, contours, cannyThreshold1, cannyThreshold2, cannyApertureSize, l2Gradient);
    cv::imwrite("contours.jpg", contours);
    
    std::vector<cv::Vec4i> lines = finder.findLines(contours);
    finder.drawDetectedLines(image, cv::Scalar(0,0,0));
    
    std::vector<std::vector<int> > levelFile = finder.saveToVec(image);
    
    return levelFile;
}

int Play::buildLevel(std::vector<std::vector<int> > levelFile)
{
    sf::Texture map;
    map.loadFromFile("assets/castleCenter_rounded.png");
    // to be continued
}

int Play::play(std::vector<std::vector<int> > levelFile)
{
    buildLevel(levelFile);
    Game game;
    int success = game.run();
    return success;
}
