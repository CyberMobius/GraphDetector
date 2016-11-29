#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	
	int r, g, b;

	Mat frame[3]; 
	Mat compareImage[3];
	Mat wholeFrame, compare;

	split(imread("c:/users/brimy/desktop/resources/Code/Personal Projects/Resources/human.jpg"), compareImage );

	for (int i = 0; i < 3; i ++) {
		namedWindow(to_string(i),WINDOW_AUTOSIZE);
		imshow(to_string(i),compareImage[i]);
	}


	VideoCapture cap(0);
	if (!cap.isOpened()) {
		return -1;
	}

	
	while (true) {
		waitKey(1);
	}

	while (true) {
		
		cap >> wholeFrame;
		split(wholeFrame, frame);
		
		

		for (int i = 0; i < 3; i++) {
			namedWindow(to_string(i), WINDOW_AUTOSIZE);
			imshow(to_string(i), frame[i]);
		}
		
		if (waitKey(1) >= 0) break;
	}
	
	//waitKey(0);

	return 0;
}