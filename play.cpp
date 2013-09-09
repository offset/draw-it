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

std::string Play::detect(std::string fileNameImage,
                   std::string fileNameLevel,
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
    
//    cv::Mat drawImage2(image.rows,image.cols, CV_8UC3,cv::Scalar(255,255,255));
//    finder.drawDetectedLines(drawImage2, cv::Scalar(0,0,0));
//    cv::imwrite("lines.jpg", drawImage2);
    
    finder.saveToDisk(image, fileNameLevel);
    
    return fileNameLevel;
}

int Play::buildLevel(std::string fileNameLevel)
{
    // loading text file which contains instructions for building the level
    std::ifstream level(fileNameLevel);
    if (!level.is_open())
    {
        std::cout << "Could not read level file." << std::endl
                  << "Now exiting." << std::endl;
        return READ_ERROR;
    }
    std::string line;
    sf::Texture map;
    map.loadFromFile("assets/castleCenter_rounded.png");
    while(std::getline(level, line))
    {
        if(line == "1")
        {
            // to be continued
        }
    }
}

int Play::play(std::string fileNameLevel)
{
    buildLevel(fileNameLevel);
    Game game;
    int success = game.run();
    return success;
}
