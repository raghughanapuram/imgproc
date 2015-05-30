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
	int c1=0, c2=0, dsum=0, dnum=0;
	bool flagtopblack=FALSE;
	bool flagbottomblack=FALSE;
	for(int i=1;i<ret.rows-1;i++) 
	{
		if(hppimg.at<int>(i,0) != 0 && hppimg.at<int>(i+1,0) == 0)
		{
			c1=0; c2=0;
			flagtopblack=FALSE;
			flagbottomblack=FALSE;
			for(int j=i;j>=0;j--) // going Top
			{
				if(hppimg.at<int>(j,thecol)==0)
				{
					flagtopblack = TRUE;
					break;
				}
				else
				{
					c1++;
					//hppimg.at<int>(j,thecol) = 70;
				}
			}

			for(int j=i+1;j<ret.rows-1;j++) // going Bottom
			{
				if(hppimg.at<int>(j,thecol)==0)
				{
					flagbottomblack = TRUE;
					break;
				}
				else
				{
					c2++;
					//hppimg.at<int>(j,thecol) = 200;
				}
			}

			if(flagtopblack == TRUE && flagbottomblack == TRUE)
			{
				dsum = dsum + (c1 + c2);
				dnum++;

				for (int k = 0; k < c1; ++k)
				{
					hppimg.at<int>(i-k,thecol) = 70;
				}
				for (int k = 0; k < c2; ++k)
				{
					hppimg.at<int>(i+1+k,thecol) = 180;
				}
			}
		}
	}
	imwrite("hppimg.tif", hppimg);
	imshow("hppimg.tif", hppimg);
	int distance = dsum / dnum;
	printf("max peak = %d, at = %d, midpoint = %d\ndsum = %d, dnum = %d, distance = %d\n", 
		max, maxrow, thecol, dsum, dnum, distance);
	//waitKey(0);
	return 0;
}
