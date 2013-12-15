#include "play.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include "errcodes.hpp"

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

LineFinder* Play::getFinder()
{
    return finder;
}

void Play::destroy()
{
    if (singleton != 0)
    {
        delete singleton;
        singleton = 0;
    }
}

void Play::detect(float minLength , 
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
    
    // creating a skeleton
    finder.createSkeleton(skelThreshold);
    
    // detecting the contours
    cv::Canny(Play::getInstance()->getFinder()->getImage(), Play::getInstance()->getFinder()->getImage(), cannyThreshold1, cannyThreshold2, cannyApertureSize, l2Gradient);
    
    std::vector<cv::Vec4i> lines = finder.findLines();
    
    std::vector<std::vector<int> > levelFile = finder.saveToVec();
    setLevelMap(levelFile);
    //int howLong = Play::getInstance()->getFinder()->getLines().size();
    assert(!levelFile.empty());
    assert(!Play::getInstance()->getLevelMap().empty());
}

void Play::setLevelMap(std::vector<std::vector<int> > lm)
{
    levelMap = lm;
}

std::vector<std::vector<int> > & Play::getLevelMap()
{
    return levelMap;
}

int Play::buildLevel()
{
    // PART 1: building the background image
    std::vector<cv::Mat> TexturesBg(2);
    cv::Mat texture = cv::imread("../drawit/assets/textures/tiles/dummy1.png");
    if(!texture.data)
    {
        std::cerr << "Texture could not be loaded..." <<
                     "Exiting." << std::endl;
        return -1;
    }
    TexturesBg[0] = texture;
    texture = cv::imread("../drawit/assets/textures/tiles/dummy2.png");
    if(!texture.data)
    {
        std::cerr << "Texture could not be loaded..." <<
                     "Exiting." << std::endl;
        return -1;
    }
    TexturesBg[1] = texture;
    int tileWidth = texture.size().width;
    int tileHeight = texture.size().height;
    int levelMapWidth = levelMap.size();
    int levelMapHeight = levelMap[0].size();
    cv::Mat bgImg(levelMap.size(), levelMap[0].size(), CV_8UC3, cv::Scalar(255,0,255));
    for (uint row = 0; row < levelMap.size(); ++ row)
    {
        for (uint col = 0; col < levelMap[0].size(); ++ col)
        {
            if(levelMap[row][col] == 0)
            {
                texture = TexturesBg[0];
                cv::Mat roi = bgImg(cv::Rect(col, row, tileWidth, tileHeight));
                texture.copyTo(roi);
            } else
            {
                texture = TexturesBg[1];
                cv::Mat roi = bgImg(cv::Rect(col, row, tileWidth, tileHeight));
                texture.copyTo(roi);
            }
        }
    }
    levelBg = bgImg;
    cv::imwrite("levelBg.png", bgImg);
    
    // IMPORTANT: This needs to be done via vertexes, DO IT!
    // PART 2: building the level environment
    if(!m_TileMap.load("../drawit/assets/textures/tiles/tiles_spritesheet_cropped.png", sf::Vector2u(70, 70), Play::getInstance()->getLevelMap(), levelMap[0].size(), levelMap.size()))
    {
        return -1; 
    }

    return 0;
}
            
int Play::play()
{
    buildLevel();
    Game game;
    int success = game.run();
    return success;
}

tileMap & Play::getTileMap()
{
    return m_TileMap;
}
