#include "linefinder.hpp"
#include <utility>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>

LineFinder::LineFinder() : img(),
    deltaRho(1), 
    deltaTheta(3.14f/180), 
    minVote(10), 
    minLength(0.f), 
    maxGap(0.f)
{
    // empty
}

void LineFinder::setAccResolution(double dRho, 
                             double dTheta)
{
    dRho = deltaRho;
    dTheta = deltaTheta;
}

void LineFinder::setMinVote(int minv)
{
    minVote = minv;
}

void LineFinder::setLineLengthAndGap(double length, 
                                double gap)
{
    minLength = length;
    maxGap = gap;
}

cv::Mat & LineFinder::getImage()
{
    return img;
}

int LineFinder::setImage(std::string filePath)
{
    img = cv::imread(filePath);
    if(!img.data)
    {
        std::cout << "File could not be loaded..." <<
                "Exiting." << std::endl;
        return -1;
    }
    return 0;
}

std::vector<cv::Vec4i> LineFinder::findLines()
{
    lines.clear();
    
    // debugging purposes
    assert(img.channels() == 1);
    
    cv::HoughLinesP(img,
                    lines,
                    deltaRho,
                    deltaTheta,
                    minVote,
                    minLength,
                    maxGap);
    
    return lines;
}

void LineFinder::drawDetectedLines( cv::Scalar color)
{
    std::vector<cv::Vec4i>::const_iterator it2 = lines.begin();
    
    while (it2 != lines.end())
    {
        cv::Point pt1((*it2)[0], (*it2)[1]);
        cv::Point pt2((*it2)[2], (*it2)[3]);
        
        cv::line(img, pt1, pt2, color);
        
        ++it2;
    }
}

void LineFinder::createSkeleton(int threshold)
{
    // the image has to be grayscale
    if (img.channels() != 1)
    {
        cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    }
    
    // we need to enhance the lighting before we can threshold the image
    cv::equalizeHist(img, img);
    // a binary image is needed
    cv::threshold(img, img, threshold, 255, cv::THRESH_BINARY_INV);
    
    // the resulting skeleton
    cv::Mat skeleton(img.size(), CV_8UC1, cv::Scalar(0,0,0));
    // needed if in-place processing is not possible
    cv::Mat temp(img.size(), CV_8UC1);
    // eroded image is saved here
    cv::Mat eroded;
    // needed for morphological transforms (erodation, dilation)
    cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));
    
    int i = 0;
    while(i != 20)
    {
        // eroding + dilating = opening
        cv::erode(img, eroded, element);
        cv::dilate(eroded, temp, element);
        cv::subtract(img, temp, temp);
        cv::bitwise_or(skeleton, temp, skeleton);
        eroded.copyTo(img);
        
        //done = (cv::countNonZero(image) == 0);
        ++i;
    }
    
    img = skeleton;
}


std::vector<std::vector<int> > LineFinder::saveToVec()
{   
    cv::Mat level;
    img.copyTo(level);
    level.setTo(cv::Scalar(255,255,255));
    // scaling level size
    if(level.cols <= 1024 && level.cols >= 512)
    {
        // scale 2
        float scale = 0.5;
        cv::resize(level, level, cv::Size(0,0), scale, scale, cv::INTER_NEAREST);
        int a = level.cols;
        int b = level.rows;
        // scaling line sizes
        // how many elements are there in Vec4i
        uint vecSize = 4;
        for (auto it = lines.begin(); it != lines.end(); ++it)
        {
            for (uint i = 0; i < vecSize; ++i)
            {
                (*it)[i] /= scale;
//                assert((*it)[i] <= level.cols
//                       && (*it)[i] <= level.rows);
            }
        }
    } else if (level.cols > 1024 && level.cols <= 2048)
    {
        // scale 4
        float scale = 0.25;
        cv::resize(level, level, cv::Size(0,0), scale, scale, cv::INTER_NEAREST);
        // scaling line sizes
        // how many elements are there in Vec4i
        uint vecSize = 4;
        for (auto it = lines.begin(); it != lines.end(); ++it)
        {
            for (uint i = 0; i < vecSize; ++i)
            {
                (*it)[i] /= scale;
                assert((*it)[i] <= level.cols
                       && (*it)[i] <= level.rows);
            }
        }
    } else if (level.cols > 2048)
    {
        // scale 6
        float scale = 0.16;
        cv::resize(level, level, cv::Size(0,0), scale, scale, cv::INTER_NEAREST);
        // scaling line sizes
        // how many elements are there in Vec4i
        uint vecSize = 4;
        for (auto it = lines.begin(); it != lines.end(); ++it)
        {
            for (uint i = 0; i < vecSize; ++i)
            {
                (*it)[i] /= scale;
                assert((*it)[i] <= level.cols
                       && (*it)[i] <= level.rows);
            }
        }
    } else if (level.cols < 512)
    {
        // scale 1
        // nothing to do
    }
    
    std::vector<std::vector<int> > levelFile;
    for (int i = 0; i < img.rows; ++i)
    {
        std::vector<int> row(img.cols);
        levelFile.push_back(row);
    }
    
    if (level.channels() != 1)
    {
        cv::cvtColor(level, level, cv::COLOR_BGR2GRAY);
    }
    drawDetectedLines(cv::Scalar(0,0,0));
    
    for (int row = 0; row < img.rows; ++row)
    {
        uchar* pixel = level.ptr<uchar>(row);
        for (int col = 0; col < img.cols; ++col)
        {
            if(static_cast<int>(*pixel) == 0)
            {
                levelFile[row][col] = 1;
            } else
            {
                levelFile[row][col] = 0;
            }
        }
    }
    img = level;
    
    return levelFile;
}
