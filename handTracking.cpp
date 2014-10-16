#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>

#include <iostream>

using namespace cv;
using namespace std;

Mat backgroundSubtraction(Mat, Mat);

int main(int argc, char* argv[])
{

	VideoCapture cap1(0);
	
	if(!cap1.isOpened())
	{
		cout << "Cannot read the camera feed!" << endl;
		return -1;
	}

	Mat frame, back, fore;
	Ptr<BackgroundSubtractorMOG2> bg = new BackgroundSubtractorMOG2(30.0,16.0,0);
	namedWindow("Original");
    namedWindow("Background");

    vector<vector<Point> > contours;

	while(1)
	{
		cap1.read(frame);
		bg->operator () (frame,fore);
		bg->getBackgroundImage(back);

		erode(fore,fore,Mat());
        dilate(fore,fore,Mat());
        findContours(fore,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
        
        int largestArea = 0, largestIndex;
    	Rect bounding_rect;

        for (int i = 0;i < contours.size();i++)
    	{
    		double a = contourArea(contours[i],false);
    		if (a > largestArea)
    		{
    			largestArea = a;
    			bounding_rect = boundingRect(contours[i]);
    			largestIndex = i;
    		}
    	}

        drawContours(frame,contours,largestIndex,Scalar(0,0,255),2);        
    	
    	rectangle(frame, bounding_rect,  Scalar(0,255,0),1, 8,0);

		// Mat object = backgroundSubtraction(frame1,frame2);
		imshow("Original",frame);
		imshow("Background",back);
		

		if(waitKey(100) == 27)
        {
            cout << "ESC is pressed!" << endl;
            break;
        }
	}

	return 0;		
}

Mat backgroundSubtraction(Mat background, Mat current)
{
	Mat frame1GRAY;
	Mat frame2GRAY;
	cvtColor(background,frame1GRAY,CV_RGB2GRAY);
	cvtColor(current,frame2GRAY,CV_RGB2GRAY);

	Mat diff(frame1GRAY.rows, frame1GRAY.cols,CV_8UC1);

	absdiff(frame2GRAY,frame1GRAY,diff);

	return diff;
}