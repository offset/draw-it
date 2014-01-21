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

int Play::detect(float minLength , 
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
    
    if (Play::getInstance()->getFinder()->getLines().size() == 0)
    {
        std::cerr << "There were no lines detected." << std::endl;
        return -1;
    }
    
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
    if(!m_TileMap.load(sf::Vector2u(5, 5), Play::getInstance()->getLevelMap(), levelMap[0].size(), levelMap.size()))
    {
        return -1;
    }

    return 0;
}
            
int Play::play()
{
    //physicsMap(levelMap);
    buildLevel();
    Game game;
    int success = game.run();
    return success;
}

tileMap & Play::getTileMap()
{
    return m_TileMap;
}

sf::Vector2u Play::getMapSize()
{
    return mapSize;
}

void Play::setMapSize(int width, int height)
{
    mapSize.x = width;
    mapSize.y = height;
}

std::vector<std::vector<int> > & Play::getPhysicsMap()
{
    return physicsMap;
}

void Play::setPhysicsMap(std::vector<std::vector<int> > newPhMap)
{
    physicsMap = newPhMap;
}
