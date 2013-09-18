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
    
    cv::Mat & getImage();
    
    int setImage(std::string filePath);
    
    std::vector<cv::Vec4i> findLines();
    
    void drawDetectedLines(cv::Scalar color = cv::Scalar(255, 255, 255));
    
    std::vector<cv::Vec4i> refineDetectedLines(float minDifference = 0.05f);
    
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
     * \param threshold[in]: optional parameter for specifing the threshold limit. The mode is always THRESH_BINARY.
     *
     * @return The created skeleton image.
     */
    void createSkeleton(int threshold = 127);
    
    /*! \author zemann
     *
     * \brief Stores the image in a format SFML can read for further processing.
     *
     * Firstly the function resizes a copy of the image (same size) and the lines depending on their size:
     * < 512px: no rescale
     * up to 1024: scale 2
     * up to 2048: scale 4
     * > 2048: scale 6
     * NOTE: These limits and scales were chosen quite randomly and may change in the next revision.
     * Then the lines are drawn on the (grayscale) copy and a sort of a 'threshold' is performed:
     * If the pixel value is unequal 0, the corresponding value in a vector of a vector of int (represents the image)
     * is set to 1 (line), otherwise to 0.
     * This may change in the future because a certain number could be used for certain terrain objects (walls, obstacles, etc.).
     *
     * @return vector of a vector of int containing the line information.
     */
    std::vector<std::vector<int> > saveToVec();
};
#endif // LINEFINDER_H
