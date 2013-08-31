#ifndef GAME_H
#define GAME_H
#include <string>
#include "linefinder.hpp"

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
    static Game* getInstance();
    
    static void destroy();
    
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
};

#endif // Game_H
