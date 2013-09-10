#ifndef LINEFINDER_H
#define LINEFINDER_H
#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>

class LineFinder
{
private:
    cv::Mat img;
    
    // this vector contains the end points
    // of detected lines
    std::vector<cv::Vec4i> lines;
    
    // resolution parameters for accumulator
    double deltaRho;
    double deltaTheta;
    
    int minVote;
    double minLength;
    double maxGap;
    
public:
    LineFinder();
    
    void setAccResolution(double dRho,
                          double dTheta);
    
    void setMinVote(int minv);
    
    void setLineLengthAndGap(double length,
                             double gap);
    
    std::vector<cv::Vec4i> findLines(cv::Mat& binary);
    
    void drawDetectedLines(cv::Mat& image,
                           cv::Scalar color = cv::Scalar(255, 255, 255),
                           int scaleFactorRows = -1,
                           int scaleFactorCols = -1);
    
    std::vector<cv::Vec4i> refineDetectedLines(float minDifference = 0.05f);
    
    void alignLinesToGrid(cv::Mat &image);
    
    /*! \author zemann
     *
     * \brief Creates a refined image by removing outer pixels of the structures in the image.
     *
     * The function makes sure that the image is grayscale, then applies a inverted threshold on it to obtain a binary image.
     * In a loop, which ends when all pixels of the image are zero, the image is opened to remove unneccessary pixels,
     * then this opened image is subtracted from the original image, which is in return copied to skeleton.
     * At last, the image is eroded. This then is the starting image for the next iteration. The skeleton is returned.
     * The function is based on this blog entry:
     * http://felix.abecassis.me/2011/09/opencv-morphological-skeleton
     *
     * \param image[in,out]: The image to use for generating the skeleton. NOTE: This image is all zero at the end.
     * \param threshold[in]: optional parameter for specifing the threshold limit. The mode is always THRESH_BINARY.
     *
     * @return The created skeleton image.
     */
    cv::Mat createSkeleton(cv::Mat& image, int threshold = 127);
    
    /*! \author zemann
     *
     * \brief Stores the image in a format SFML can read for further processing.
     *
     * \param image[in]: The image to be saved
     *
     * @return vector containing the line information.
     */
    std::vector<std::vector<int> > saveToVec(cv::Mat image);
};
#endif // LINEFINDER_H
