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
    deltaRho = dRho;
    deltaTheta = dTheta;
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

std::vector<cv::Vec4i>& LineFinder::getLines()
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
    // debugging
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
    for (uint i = 0; i < Play::getInstance()->getFinder()->getLines().size(); ++i)
    {
        cv::Point pt1(Play::getInstance()->getFinder()->getLines()[i][0], Play::getInstance()->getFinder()->getLines()[i][1]);
        cv::Point pt2(Play::getInstance()->getFinder()->getLines()[i][2], Play::getInstance()->getFinder()->getLines()[i][3]);
        
        cv::line(Play::getInstance()->getFinder()->getImage(), pt1, pt2, color);
    }
}

void LineFinder::createSkeleton(int threshold)
{
    // the image has to be grayscale
    if (Play::getInstance()->getFinder()->getImage().channels() != 1)
    {
        cv::cvtColor(Play::getInstance()->getFinder()->getImage(), Play::getInstance()->getFinder()->getImage(), cv::COLOR_BGR2GRAY);
    }
    
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
    // scaling level size
    if(level.cols <= 1024 && level.cols >= 512)
    {
        // scale 2
        float scale = 0.5;
        cv::resize(level, level, cv::Size(0,0), scale, scale, cv::INTER_NEAREST);
        // scaling line sizes
        // how many elements are there in Vec4i?
        uint vecSize = 4;
        // scales to lines which are to be drawn
        for (auto it = lines.begin(); it != lines.end(); ++it)
        {
            for (uint i = 0; i < vecSize; ++i)
            {
                (*it)[i] /= scale;
//                assert((*it)[i] <= level.cols
//                       && (*it)[i] <= level.rows);
            }
        }
        cv::imwrite("scaledImage.png", level);
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
    
    if (level.channels() != 1)
    {
        cv::cvtColor(level, level, cv::COLOR_BGR2GRAY);
    }
    
    Play::getInstance()->getFinder()->setImage(level);
    
    std::vector<std::vector<int> > levelFile;
    
    for (int i = 0; i < Play::getInstance()->getFinder()->getImage().rows; ++i)
    {
        std::vector<int> row(Play::getInstance()->getFinder()->getImage().cols);
        for (int j = 0; j < Play::getInstance()->getFinder()->getImage().cols; ++j)
        {
            row[j] = Play::getInstance()->getFinder()->getImage().at<int>(i, j);
        }
        levelFile.push_back(row);
    }

    // debug
    std::ofstream os("levelFile.txt");

    for(uint i = 0; i < levelFile.size(); ++i)
    { 
        for (uint j = 0; j < levelFile[0].size(); ++j)
        {
            if(levelFile[i][j] == 0)
            {
                levelFile[i][j] = 0;
                os << 0;
            } else
            {
                levelFile[i][j] = 1;
                os << 1;
            }
        }
        os << std::endl;
    }
    
    os.close();

    Play::getInstance()->setPhysicsMap(levelFile);

    // debug
//    cv::Mat vecToMat(levelFile.size(), levelFile[0].size(), CV_8UC1);
//    for(int i = 0; i < levelFile.size(); ++i)
//    {
//        for(int j = 0; j < levelFile[0].size(); ++j)
//        {
//            vecToMat.at<int>(i, j);
//        }
//    }
//    cv::imwrite("vecToImage.png", vecToMat);

    Play::getInstance()->getFinder()->setImage(level);
    
    return levelFile;
}

void LineFinder::drawLinePoints()
{
    //cv::Mat img = Play::getInstance()->getFinder()->getImage();
    cv::Mat img;
    img.create(Play::getInstance()->getFinder()->getImage().rows, Play::getInstance()->getFinder()->getImage().cols, CV_8UC3);
    //img = cv::imread("../level.png");
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
