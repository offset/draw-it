#include <iostream>
#include <string>
#include <fstream>
#include "errcodes.hpp"
#include <linefinder.hpp>
#include "play.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;

int main()
{   
    Play::getInstance()->detect();
    Play::getInstance()->play();
    Play::getInstance()->destroy();
}
