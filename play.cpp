#include "play.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include "errcodes.hpp"
//#define DEBUG_GAME

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

int Play::detect(float minLength,
                   float mGap, 
                   int minVote,
                   int skelThreshold,
                   int cannyThreshold1,
                   int cannyThreshold2,
                   int cannyApertureSize,
                   bool l2Gradient
                   )
{
    LineFinder finder;
    finder.setLineLengthAndGap(minLength, maxGap);
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
    
    // This closing operation fills holes between lines
    // It has a very significant effect
    cv::Mat tempDilated;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));
    cv::dilate(Play::getInstance()->getFinder()->getImage(), tempDilated, element);
    cv::erode(tempDilated, Play::getInstance()->getFinder()->getImage(), element);
    
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
#ifdef DEBUG_GAME
    levelWidth = 100;
    levelHeight = 50;
    std::vector<int> obstacleRow = {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
    };
    assert(obstacleRow.size() == 100);
    std::vector<int> freeRow = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };
    assert(freeRow.size() == 100);
    std::vector<int> randomRow = {
        1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0
    };
    assert(randomRow.size() == 100);
    std::vector<std::vector<int> > debugLV = {
        freeRow, freeRow, freeRow, freeRow, freeRow,
        freeRow, freeRow, freeRow, freeRow, freeRow,
        randomRow, randomRow, randomRow, randomRow, randomRow,
        randomRow, randomRow, randomRow, randomRow, randomRow,
        randomRow, randomRow, randomRow, randomRow, randomRow,
        randomRow, randomRow, randomRow, randomRow, randomRow,
        randomRow, randomRow, randomRow, randomRow, randomRow,
        obstacleRow, obstacleRow, obstacleRow, obstacleRow, obstacleRow,
        obstacleRow, obstacleRow, obstacleRow, obstacleRow, obstacleRow,
        obstacleRow, obstacleRow, obstacleRow, obstacleRow, obstacleRow
    };
    assert(debugLV.size() == 50);
    Play::getInstance()->setLevelMap(debugLV);
    Play::getInstance()->setPhysicsMap(debugLV);
#endif
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

float Play::getMinLength()
{
    return minLength;
}

void Play::setMinLength(float mLength)
{
    minLength = mLength;
}

float Play::getMaxGap()
{
    return maxGap;
}

void Play::setMaxGap(float mGap)
{
    maxGap = mGap;
}

int Play::getMinVote()
{
    return minVote;
}

void Play::setMinVote(int mVote)
{
    minVote = mVote;
}

int Play::getSkelThreshold()
{
    return skelThreshold;
}

void Play::setSkelThreshold(int skThreshold)
{
    skelThreshold = skThreshold;
}

int Play::getCannyThreshold1()
{
    return cannyThreshold1;
}

void Play::setCannyThreshold1(int cThreshold1)
{
    cannyThreshold1 = cThreshold1;
}

int Play::getCannyThreshold2()
{
    return cannyThreshold2;
}

void Play::setCannyThreshold2(int cThreshold2)
{
    cannyThreshold2 = cThreshold2;
}

int Play::getCannyApertureSize()
{
    return cannyApertureSize;
}

void Play::setCannyApertureSize(int cASize)
{
    cannyApertureSize = cASize;
}

bool Play::getL2Gradient()
{
    return l2Gradient;
}

void Play::setL2Gradient(bool l2Grad)
{
    l2Gradient = l2Grad;
}
