/*
 * Horizontal Projection Profile
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
	int max = 0; 
	int maxrow = 0;
    for(int i=0;i<ret.rows;i++)
    {
        vertical.at<int>(i,0) = countNonZero(ret(Rect(0,i,ret.cols,1)));
        for (int j = 0; j < vertical.at<int>(i,0); ++j)
        {
            hppimg.at<int>(i,j) = 0;
        }
		if ( max < vertical.at<int>(i,0) )
		{
			max = vertical.at<int>(i,0);
			maxrow = i;
		}
    }

	int thecol = max / 2;
	/*
	for(int i=0;i<ret.rows;i++)
    {
		hppimg.at<int>(i,thecol) = 230;
	}
	*/

	//Mat hppc = imread(hppimg, CV_LOAD_IMAGE_COLOR);
	int c1=0, c2=0, dsum=0, dnum=0;
	bool flagtopblack=FALSE;
	bool flagbottomblack=FALSE;
	for(int i=1;i<ret.rows-1;i++) 
	{
		if(hppimg.at<int>(i,0) != 0 && hppimg.at<int>(i+1,0) == 0)
		{
            flagtopblack
			printf("row = %d\n", i);
			c1=0; c2=0;
			for(int j=i;hppimg.at<int>(j,thecol)!=0 && j>=0;j--)
			{
				c1++;
				hppimg.at<int>(j,thecol) = 30;
			}
			for(int j=i+1;hppimg.at<int>(j,thecol)!=0 && j<ret.rows-1;j++)
			{
				c2++;
				hppimg.at<int>(j,thecol) = 200;
			}
			dsum = dsum + (c1 + c2);
			dnum++;
		}
	}
    imwrite("hppimg.tif", hppimg);
    imshow("hppimg.tif", hppimg);
	int distance = dsum / dnum;
	printf("max peak = %d, at = %d, midpoint = %d, distance = %d\n", max, maxrow, thecol, distance);
    //waitKey(0);CV_RGB(255, 0, 0)
    return 0;
}
