#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	
	system("color a");

	Mat frame;

	VideoCapture cap(0);
	if (!cap.isOpened()) {
		return -1;
	}

	namedWindow("Video",WINDOW_AUTOSIZE);
	//namedWindow("Grey Video", WINDOW_AUTOSIZE);
	//namedWindow("Raw Video", WINDOW_AUTOSIZE);

	while (true) {
		
		cap >> frame;
		//imshow("Raw Video", frame);


		cvtColor(frame,frame,CV_BGR2GRAY);
		//imshow("Grey Video", frame);


		GaussianBlur(frame,frame,Size(3,3),0,0);
		
		Canny(frame,frame,600,1500,5,false);   
		imshow("Video", frame);
		
		if (waitKey(1) >= 0) break;
	}

	//waitKey(0);

	return 0;
}