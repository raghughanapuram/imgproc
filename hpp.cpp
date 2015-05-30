/**
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
{	// Reading image from commandline-argument
	Mat img = imread(argv[1]);
	Mat gray;
	cvtColor(img,gray,CV_BGR2GRAY);
	//To store the thresholded image
	Mat ret;
	//threshold the image
	threshold(gray,ret,0,255,CV_THRESH_BINARY_INV+CV_THRESH_OTSU);

	Mat vertical(ret.rows,1,CV_8U); //vertical histogram    
	vertical = Scalar::all(0);

	Mat hppimg(ret.rows, ret.cols, CV_8U); // Horizontal Projection
	hppimg = Scalar::all(255); // Filling image with a fixed value

	int max = 0; 
	int maxrow = 0;
	for(int i=0;i<ret.rows;i++)
	{
		vertical.at<uchar>(i,0) = countNonZero(ret(Rect(0,i,ret.cols,1)));
		for (int j = 0; j < vertical.at<uchar>(i,0); ++j)
		{
			hppimg.at<uchar>(i,j) = 0;
		}
		if ( max < vertical.at<uchar>(i,0) )
		{
			max = vertical.at<uchar>(i,0);
			maxrow = i;
		}
	}
	
	Mat hppimgc(hppimg.size(), CV_8UC3);
	cvtColor(hppimg, hppimgc, CV_GRAY2BGR);

	int thecol = max / 2;
	int c1=0, c2=0, dsum=0, dnum=0;
	bool flagtopblack=FALSE;
	bool flagbottomblack=FALSE;
	for(int i=1;i<ret.rows-1;i++) 
	{
		if(hppimg.at<uchar>(i,0) != 0 && hppimg.at<uchar>(i+1,0) == 0)
		{
			c1=0; c2=0;
			flagtopblack=FALSE;
			flagbottomblack=FALSE;
			for(int j=i;j>=0;j--) // going Top
			{
				if(hppimg.at<uchar>(j,thecol)==0)
				{
					flagtopblack = TRUE;
					break;
				}
				else
				{
					c1++;
					//hppimg.at<uchar>(j,thecol) = 70;
				}
			}

			for(int j=i+1;j<ret.rows-1;j++) // going Bottom
			{
				if(hppimg.at<uchar>(j,thecol)==0)
				{
					flagbottomblack = TRUE;
					break;
				}
				else
				{
					c2++;
					//hppimg.at<uchar>(j,thecol) = 200;
				}
			}

			if(flagtopblack == TRUE && flagbottomblack == TRUE)
			{
				dsum = dsum + (c1 + c2);
				dnum++;

				for (int k = 0; k < c1; ++k)
				{
					//hppimg.at<uchar>(i-k,thecol) = 70;
					hppimgc.at<Vec3b>(i-k,thecol) = Vec3b(255,0,0);
				}
				for (int k = 0; k < c2; ++k)
				{
					//hppimg.at<uchar>(i+1+k,thecol) = 180;
					hppimgc.at<Vec3b>(i+1+k,thecol) = Vec3b(0,0,255);
				}
			}
		}
	}
	imwrite("hppimg.tif", hppimg);
	imwrite("hppimgc.tif", hppimgc);
	//imshow("hppimg.tif", hppimg);
	int distance = dsum / dnum;
	printf("max peak = %d, at = %d, midpoint = %d\ndsum = %d, dnum = %d, distance = %d\n", 
		max, maxrow, thecol, dsum, dnum, distance);
	//waitKey(0);
	return 0;
}
