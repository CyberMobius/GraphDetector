#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <stdio.h>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	namedWindow("Picture", WINDOW_AUTOSIZE);

	Mat frame = imread("C:/Users/brimy/OneDrive/Pictures/Hamiltonex.jpg");
	imshow("Picture", frame);
	

	vector<Vec3f> nodes;

	

	cvtColor(frame, frame, CV_RGB2GRAY);

	HoughCircles(frame, nodes, CV_HOUGH_GRADIENT, 2, 100);

	Mat verticies(frame.size(), CV_8U);

	for (size_t i = 0; i < nodes.size(); i++) {
		Point center(cvRound(nodes[i][0]), cvRound(nodes[i][1]));
		int radius = cvRound(nodes[i][2]);

		circle(verticies, center, radius, Scalar(255,255,255), 4);

	}

	//Canny(frame,edges,300,800,5);
	imshow("Picture", verticies);


	waitKey(0);

	return 0;
}
