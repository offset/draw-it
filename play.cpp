#include "play.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <SFML/Graphics/Sprite.hpp>
#include <errcodes.hpp>

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
    cv::Canny(finder.getImage(), finder.getImage(), cannyThreshold1, cannyThreshold2, cannyApertureSize, l2Gradient);
    
    std::vector<cv::Vec4i> lines = finder.findLines();
    
    std::vector<std::vector<int> > levelFile = finder.saveToVec();
    setLevelMap(levelFile);
}

void Play::setLevelMap(std::vector<std::vector<int> > lm)
{
    levelMap = lm;
}

std::vector<std::vector<int> > & Play::getLevelMap()
{
    return levelMap;
}

sf::Image Play::buildLevel()
{
    std::vector<sf::Texture> Textures(2);
    sf::Texture texture;
    if(!texture.loadFromFile("../drawit/assets/textures/tiles/castleCenter.png"))
    {
        std::cout << "Could not load texture." << std::endl <<
                     "Now exiting." << std::endl;
        exit(READ_ERROR);
    }
    Textures[0] = texture;
    if(!texture.loadFromFile("../drawit/assets/textures/tiles/sky.png"))
    {
        std::cout << "Could not load texture." << std::endl <<
                     "Now exiting." << std::endl;
        exit(READ_ERROR);
    }
    Textures[1] = texture;
    int tileWidth = 10;
    int tileHeight = 10;
    level.create(levelMap[0].size()*tileWidth, levelMap[0].size()*tileHeight);
    level.setSmooth(true);
    level.clear();
    sf::Sprite tile;
    // building the level
    for (uint row = 0; row < levelMap.size(); ++ row)
    {
        for (uint col = 0; col < levelMap[0].size(); ++ col)
        {
            if(levelMap[row][col] == 0)
            {
                tile.setTexture(Textures[0]);
                tile.setPosition(static_cast<float>(row*tileWidth/2),
                                 static_cast<float>(col*tileHeight/2));
            } else
            {
                tile.setTexture(Textures[1]);
                tile.setPosition(static_cast<float>(row*tileWidth/2),
                                 static_cast<float>(col*tileHeight/2));
            }
            level.draw(tile);
        }
    }
}

int Play::play()
{
    buildLevel();
    Game game;
    int success = game.run();
    return success;
}
