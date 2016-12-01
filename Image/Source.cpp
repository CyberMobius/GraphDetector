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
	namedWindow("Lines", WINDOW_AUTOSIZE);

	Mat frame = imread("C:/Users/brimy/OneDrive/Pictures/graph.png");
	imshow("Picture", frame);
	

	vector<Vec3f> nodes;
	vector<Vec4i> u;

	cvtColor(frame, frame, CV_RGB2GRAY);
	
	Mat edges(frame.size(), CV_8U);

	Canny(frame, edges, 300, 800, 5);

	HoughCircles(frame, nodes, CV_HOUGH_GRADIENT, 2, 150);

	Mat verticies(frame.size(), CV_8U);

	for (size_t i = 0; i < nodes.size(); i++) {
		Point center(cvRound(nodes[i][0]), cvRound(nodes[i][1]));
		nodes[i][2] = 1.25*cvRound(nodes[i][2]);
		int radius = nodes[i][2];

		circle(verticies, center, radius, Scalar(0,0,255), 4);
	}

	HoughLinesP(edges, u, 1, CV_PI / 180, 50,50,10);
	Mat lines(frame.size(), CV_8U);

	for (size_t i = 0; i < u.size(); i++) {
		Vec4i l = u[i];
		line(verticies, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
		
	}
	//imshow("Lines",lines);
	imshow("Picture", verticies);

	waitKey(0);

	return 0;
}
