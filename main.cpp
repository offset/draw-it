#include <iostream>
#include <string>
#include "linefinder.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;

int main()
{   
    LineFinder finder;
    finder.setLineLengthAndGap(100.f,40.f);
    finder.setMinVote(80);
    
    cv::Mat image = cv::imread("../level.png");
    if(!image.data)
    {
        cout << "File could not be loaded..." <<
                "Exiting." << endl;
        return -1;
    }
    
    // creating a skeleton
    image = finder.createSkeleton(image, 50);
    finder.saveToDisk(image);
    
    // detecting the contours
    cv::Mat contours;
    cv::Canny(image, contours, 40, 300, 3, true);
    cv::imwrite("contours.jpg", contours);
    
    vector<cv::Vec4i> lines = finder.findLines(contours);
    cv::Mat drawImage2(image.rows,image.cols, CV_8UC3,cv::Scalar(255,255,255));
    finder.drawDetectedLines(drawImage2, cv::Scalar(0,0,0));
    cv::imwrite("lines.jpg", drawImage2);
    
    finder.saveToDisk(image);
}
