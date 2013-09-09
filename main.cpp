#include <iostream>
#include <string>
#include <fstream>
#include "errcodes.h"
#include <linefinder.hpp>
#include "play.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;

int main()
{   
    std::string fileNameLevel = Play::getInstance()->detect();
    Play::getInstance()->destroy();
    Play::getInstance()->play(fileNameLevel);
}
