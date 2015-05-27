/*
 * Horizontal Projection Profile
 * Usage:
 * ./hpp <input-image-file>
 */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    /*
    IplImage* img = cvLoadImage(argv[1]);
    IplImage* gray = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
    cvCvtColor(img, gray, CV_RGB2GRAY);
    //cvThreshold(gray, binary, 200.0, 255.0, cv::THRESH_BINARY);
    IplImage* img1 = cvCloneImage(gray);

    // find the vertical projection
    std::vector<CvRect> hpp; // holds the column sum values    
    //double max_vl_proj = 0; // holds the maximum value    
    for( int i=0;i<img1->height;++i )
    {
        CvMat row;
        CvScalar row_sum;
        cvGetRow( img, &row, i ); // get individual columns    
        row_sum = cvSum( &row ); // find the sum of ith column
        hpp.push_back( row_sum.val[0] ); // push back to vector
    }
    */


    Mat img = imread(argv[1]);
    Mat gray;
    cvtColor(img,gray,CV_BGR2GRAY);
    //To store the thresholded image
    Mat ret;
    //threshold the image
    threshold(gray,ret,0,255,CV_THRESH_BINARY_INV+CV_THRESH_OTSU);

    Mat vertical(ret.rows,1,CV_32S); //vertical histogram    
    vertical = Scalar::all(0);

    Mat hppimg(ret.rows, ret.cols, CV_32S); // Horizontal Projection
    hppimg = Scalar::all(255); // Filling image with a fixed value

    /*
    Mat horizontal(ret.cols,1,CV_32S); //horizontal histogram
    horizontal = Scalar::all(0);
    for(int i=0;i<ret.cols;i++)
    {
        horizontal.at<int>(i,0)=countNonZero(ret(Rect(i,0,1,ret.rows)));
    }
    */

    for(int i=0;i<ret.rows;i++)
    {
        vertical.at<int>(i,0) = countNonZero(ret(Rect(0,i,ret.cols,1)));
        for (int j = 0; j < vertical.at<int>(i,0); ++j)
        {
            hppimg.at<int>(i,j) = 0;
        }
    }

    imwrite("hppimg.tif", hppimg);
    imshow("hppimg.tif", hppimg);
    //waitKey(0);
    return 0;
}
