#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <algorithm>  
#include <math.h> 
#include <String.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{


	return 1;
}

void process(String path = "../graph.png") {
	namedWindow("Picture", WINDOW_AUTOSIZE);
	namedWindow("Lines", WINDOW_AUTOSIZE);

	Mat frame = imread(path);
	imshow("Picture", frame);


	vector<Vec3f> v;
	vector<Vec4i> u;

	cvtColor(frame, frame, CV_RGB2GRAY);

	Mat edges(frame.size(), CV_8U);

	Canny(frame, edges, 300, 800, 5);

	HoughCircles(frame, v, CV_HOUGH_GRADIENT, 2, 150);

	Mat verticies(frame.size(), CV_8U);

	for (size_t i = 0; i < v.size(); i++) {
		Point center(cvRound(v[i][0]), cvRound(v[i][1]));
		v[i][2] = 1.25*cvRound(v[i][2]);
		int radius = v[i][2];

		circle(verticies, center, radius, Scalar(0, 0, 255), 4);
	}

	HoughLinesP(edges, u, 1, CV_PI / 180, 50, 50, 10);
	Mat lines(frame.size(), CV_8U);

	for (size_t i = 0; i < u.size(); i++) {

		Vec4i l = u[i];
		line(verticies, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);

	}
	//imshow("Lines",lines);
	imshow("Picture", verticies);
	Point v1;
	Point v2;

	for (int i = 0; i < v.size(); i++) {
		for (int j = i + 1; i < v.size(); i++) {

			if (v[i][0] < v[j][0]) {
				v1 = Point(v[i][0], v[i][1]);
				v2 = Point(v[j][0], v[j][1]);
			}
			else {
				v2 = Point(v[i][0], v[i][1]);
				v1 = Point(v[j][0], v[j][1]);
			}

			for (int k = 0; k < u.size(); k++) {



			}
		}
	}

	waitKey(0);
}

bool insideQuad(Point corners[4],Point test) {
	double lengths[4];

	for (int i = 0; i < 4; i ++) {
		
		lengths[i] = dist(corners[i],test);
	}

	double area = 0;
	for (int i = 0; i < 4; i ++) {
		area = area + heron(lengths[i], lengths[(i+1)%4],dist(corners[i],corners[(i+1)%4]));
	}




	
}

double heron(double a, double b, double c) {
	double semisum = 0;
	
	double sides[3] = { a,b,c };

	for (int i = 1; i < 3;i++) {
		semisum = semisum + sides[i] / 2;
	
	}

	double inside = semisum;

	for (int i = 1; i < 3; i++) {
		inside = inside * (semisum - sides[i]);
	}

	return sqrt(inside);
}

double dist(Point a, Point b) {
	double x = pow(double(a.x - b.x), 2);
	double y = pow(double(a.y - b.y), 2);
	return sqrt(x+y);
}