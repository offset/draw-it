#include <iostream>
#include <string>
#include <fstream>
#include <linefinder.hpp>
#include "game.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;

int main()
{   
    std::string fileNameLevel = Game::getInstance()->detect();
    Game::getInstance()->destroy();
    Game::getInstance()->play(fileNameLevel);
}
