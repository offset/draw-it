#include "linefinder.hpp"
#include <utility>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include <play.hpp>

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

std::vector<cv::Vec4i> LineFinder::getLines()
{
    return lines;
}

int LineFinder::setImage(std::string filePath)
{
    img = cv::imread(filePath);
    if(!img.data)
    {
        std::cerr << "File could not be set..." <<
                "Exiting." << std::endl;
        return -1;
    }
    return 0;
}

int LineFinder::setImage(cv::Mat image)
{
    img = image;
    if(!img.data)
    {
        std::cout << "File could not be set..." <<
                "Exiting." << std::endl;
        return -1;
    }
    return 0;
}

std::vector<cv::Vec4i> LineFinder::findLines()
{
    Play::getInstance()->getFinder()->lines.clear();
    // debugging purposes
    assert(Play::getInstance()->getFinder()->getImage().channels() == 1);
    
    cv::HoughLinesP(Play::getInstance()->getFinder()->getImage(),
                    Play::getInstance()->getFinder()->lines,
                    deltaRho,
                    deltaTheta,
                    minVote,
                    minLength,
                    maxGap);
    
    // debugging
    for (auto it = Play::getInstance()->getFinder()->lines.begin(); it != Play::getInstance()->getFinder()->lines.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
    
    // debugging
    drawLinePoints();
    
    return lines;
}

void LineFinder::drawDetectedLines( cv::Scalar color)
{
    //std::vector<cv::Vec4i>::const_iterator it2 = lines.begin();
    std::vector<cv::Vec4i>::const_iterator it2 = Play::getInstance()->getFinder()->getLines().begin();
    
    
    while (it2 != Play::getInstance()->getFinder()->getLines().end())
    {
        cv::Point pt1((*it2)[0], (*it2)[1]);
        cv::Point pt2((*it2)[2], (*it2)[3]);
        
        cv::line(Play::getInstance()->getFinder()->getImage(), pt1, pt2, color);
        
        ++it2;
    }
    cv::imwrite("drawDetectedLines.jpg", Play::getInstance()->getFinder()->getImage());
}

void LineFinder::createSkeleton(int threshold)
{
    // the image has to be grayscale
    if (Play::getInstance()->getFinder()->getImage().channels() != 1)
    {
        cv::cvtColor(Play::getInstance()->getFinder()->getImage(), Play::getInstance()->getFinder()->getImage(), cv::COLOR_BGR2GRAY);
    }
    //Play::getInstance()->getFinder()->getImage().setTo(cv::Scalar(0,0,0));
    
    // we need to enhance the lighting before we can threshold the image
    cv::equalizeHist(Play::getInstance()->getFinder()->getImage(), Play::getInstance()->getFinder()->getImage());
    // a binary image is needed
    cv::threshold(Play::getInstance()->getFinder()->getImage(), Play::getInstance()->getFinder()->getImage(), threshold, 255, cv::THRESH_BINARY_INV);
    
    // the resulting skeleton
    cv::Mat skeleton(Play::getInstance()->getFinder()->getImage().size(), CV_8UC1, cv::Scalar(0,0,0));
    // needed if in-place processing is not possible
    cv::Mat temp(Play::getInstance()->getFinder()->getImage().size(), CV_8UC1);
    // eroded image is saved here
    cv::Mat eroded;
    // needed for morphological transforms (erodation, dilation)
    cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));
    
    int i = 0;
    while(i != 20)
    {
        // eroding + dilating = opening
        cv::erode(Play::getInstance()->getFinder()->getImage(), eroded, element);
        cv::dilate(eroded, temp, element);
        cv::subtract(Play::getInstance()->getFinder()->getImage(), temp, temp);
        cv::bitwise_or(skeleton, temp, skeleton);
        eroded.copyTo(Play::getInstance()->getFinder()->getImage());
        
        //done = (cv::countNonZero(image) == 0);
        ++i;
    }
    
    Play::getInstance()->getFinder()->setImage(skeleton);
}


std::vector<std::vector<int> > LineFinder::saveToVec()
{   
    cv::Mat level;
    Play::getInstance()->getFinder()->getImage().copyTo(level);
    //level.setTo(cv::Scalar(255,255,255));
    // scaling level size
    if(level.cols <= 1024 && level.cols >= 512)
    {
        // scale 2
        float scale = 0.5;
        cv::resize(level, level, cv::Size(0,0), scale, scale, cv::INTER_NEAREST);
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
    for (int i = 0; i < Play::getInstance()->getFinder()->getImage().rows; ++i)
    {
        std::vector<int> row(Play::getInstance()->getFinder()->getImage().cols);
        levelFile.push_back(row);
    }
    
    if (level.channels() != 1)
    {
        cv::cvtColor(level, level, cv::COLOR_BGR2GRAY);
    }
    Play::getInstance()->getFinder()->drawDetectedLines();
    //drawDetectedLines(cv::Scalar(0,0,0));
    
    for (int row = 0; row < Play::getInstance()->getFinder()->getImage().rows; ++row)
    {
        uchar* pixel = level.ptr<uchar>(row);
        for (int col = 0; col < Play::getInstance()->getFinder()->getImage().cols; ++col)
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
    Play::getInstance()->getFinder()->setImage(level);
    
    return levelFile;
}

void LineFinder::drawLinePoints()
{
    //cv::Mat img = Play::getInstance()->getFinder()->getImage();
    cv::Mat img;
    img.create(Play::getInstance()->getFinder()->getImage().rows, Play::getInstance()->getFinder()->getImage().cols, CV_8UC3);
    img = cv::imread("../level.png");
    img.setTo(cv::Scalar(0,0,0));
    
    std::vector<cv::Vec4i> lineCoords = Play::getInstance()->getFinder()->getLines();
    
    for (uint i = 0; i < lineCoords.size(); ++i)
    {
        cv::circle(img, cv::Point(lineCoords[i][0], lineCoords[i][1]), 4, cv::Scalar(0,0,255));
        cv::circle(img, cv::Point(lineCoords[i][2], lineCoords[i][3]), 4, cv::Scalar(255,0,0));
        cv::line(img, cv::Point(lineCoords[i][0], lineCoords[i][1]),
                cv::Point(lineCoords[i][2], lineCoords[i][3]),
                cv::Scalar(0,255,0));
    }
    
    cv::imwrite("pointsDrawn.jpg", img);
}
