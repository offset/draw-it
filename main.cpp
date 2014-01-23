#include <iostream>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <QApplication>
#include "play.hpp"
#include "mainwindow.hpp"
#include "settingsdialog.hpp"

using namespace std;

int main(int argc, char* argv[])
{   
    QApplication a(argc, argv);
    MainWindow w;
    
    w.show();
    
    return a.exec();
    
    Play::getInstance()->destroy();
}
