#include "game.hpp"
#include <iostream>
#include <vector>

Game* Game::singleton = 0;

Game::~Game()
{
    delete finder;
}

Game* Game::getInstance()
{
    if (singleton == 0)
    {
        singleton = new Game();
    }
    return singleton;
}

void Game::destroy()
{
    if (singleton != 0)
    {
        delete singleton;
        singleton = 0;
    }
}

std::string Game::detect(std::string fileNameImage,
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
    cv::Mat drawImage2(image.rows,image.cols, CV_8UC3,cv::Scalar(255,255,255));
    finder.drawDetectedLines(drawImage2, cv::Scalar(0,0,0));
    cv::imwrite("lines.jpg", drawImage2);
    
    finder.saveToDisk(image, fileNameLevel);
    
    return fileNameLevel;
}
