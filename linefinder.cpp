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

std::vector<cv::Vec4i> LineFinder::findLines(cv::Mat &binary)
{
    lines.clear();
    
    cv::HoughLinesP(binary,
                    lines,
                    deltaRho,
                    deltaTheta,
                    minVote,
                    minLength,
                    maxGap);
    
    return lines;
}

void LineFinder::drawDetectedLines(cv::Mat &image, 
                                   cv::Scalar color, int scaleFactorRows, int scaleFactorCols)
{
    std::vector<cv::Vec4i>::const_iterator it2 = lines.begin();
    
    if (scaleFactorRows != -1 || scaleFactorCols != -1)
    {
        while (it2 != lines.end())
        {
            cv::Point pt1((*it2)[0]/scaleFactorCols, (*it2)[1]/scaleFactorRows);
            cv::Point pt2((*it2)[2]/scaleFactorCols, (*it2)[3]/scaleFactorRows);
            
            cv::line(image, pt1, pt2, color);
            
            ++it2;
        }
    } else {
        while (it2 != lines.end())
        {
            cv::Point pt1((*it2)[0], (*it2)[1]);
            cv::Point pt2((*it2)[2], (*it2)[3]);
            
            cv::line(image, pt1, pt2, color);
            
            ++it2;
        }
    }
}

cv::Mat LineFinder::createSkeleton(cv::Mat& image, int threshold)
{
    // the image has to be grayscale
    if (image.channels() != 1)
    {
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    }
    
    // we need to enhance the lighting before we can threshold the image
    cv::equalizeHist(image, image);
    // a binary image is needed
    cv::threshold(image, image, threshold, 255, cv::THRESH_BINARY_INV);
    cv::imwrite("thresholdedImage.jpg", image);
    
    // the resulting skeleton
    cv::Mat skeleton(image.size(), CV_8UC1, cv::Scalar(0,0,0));
    // needed if in-place processing is not possible
    cv::Mat temp(image.size(), CV_8UC1);
    // eroded image is saved here
    cv::Mat eroded;
    // needed for morphological transforms (erodation, dilation)
    cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));
    
    //bool done = true;
    int i = 0;
    while(i != 20)
    {
        // eroding + dilating = opening
        cv::erode(image, eroded, element);
        cv::dilate(eroded, temp, element);
        cv::subtract(image, temp, temp);
        cv::bitwise_or(skeleton, temp, skeleton);
        eroded.copyTo(image);
        
        //done = (cv::countNonZero(image) == 0);
        ++i;
    }
    cv::imwrite("skeleton.jpg", skeleton);
    return skeleton;
}


std::vector<std::vector<int> > LineFinder::saveToVec(cv::Mat image)
{   
    cv::Mat level;
    image.copyTo(level);
    level.setTo(cv::Scalar(255,255,255));
    // scaling level size
    if(level.cols <= 1024 && level.cols >= 512)
    {
        // scale 2
        int scale = 2;
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
    } else if (level.cols > 1024 && level.cols <= 2048)
    {
        // scale 4
        int scale = 4;
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
        int scale = 6;
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
    for (int i = 0; i < image.rows; ++i)
    {
        std::vector<int> row(image.cols);
        levelFile.push_back(row);
    }
    
    if (level.channels() != 1)
    {
        cv::cvtColor(level, level, cv::COLOR_BGR2GRAY);
    }
    drawDetectedLines(level, cv::Scalar(0,0,0));
    
    for (int row = 0; row < image.rows; ++row)
    {
        uchar* pixel = level.ptr<uchar>(row);
        for (int col = 0; col < image.cols; ++col)
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
    return levelFile;
}
