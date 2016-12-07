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

const int HERON = 0;
const int AB_SIN = 1;

void process(String path = "../graph.png") {
	
	//
	namedWindow("Picture", WINDOW_AUTOSIZE);
	namedWindow("Lines", WINDOW_AUTOSIZE);

	//Shows picture from read location
	Mat frame = imread(path);
	imshow("Picture", frame);

	/*
	
	These vectors are named after graph theory notation where
	edges are referred  to by U and verticies V

	*/
	vector<Vec3f> v;
	vector<Vec4i> u;

	//Converts picture to greyscale for processing
	cvtColor(frame, frame, CV_RGB2GRAY);

	//Creates a new Mat for holding edges detected in the picture
	Mat edges(frame.size(), CV_8U);
	
	/*
	
	Bigger sizes of the Sobel filter are better at picking up text
	which is usually high contrast

	300 and 800 were determined empirically
	*/
	//Runs edge detect on the picture
	Canny(frame, edges, 300, 800, 5);

	//Detects circles in the image and writes them onto v
	HoughCircles(frame, v, CV_HOUGH_GRADIENT, 2, 150);

	//Creates a picture on which v will be drawn
	//Useful for visualization
	Mat verticies(frame.size(), CV_8U);

	//Runs through every point in V and draws them on the image
	for (size_t i = 0; i < v.size(); i++) {

		Point center(cvRound(v[i][0]), cvRound(v[i][1]));

		//Makes the radius a little bit bigger to increase the chances of finding a hard to see edge
		v[i][2] = 1.25*cvRound(v[i][2]);
		int radius = v[i][2];

		circle(verticies, center, radius, Scalar(0, 0, 255), 4);
	}

	/*
	
	HoughLinesP is more useful than HoughLines because HoughLinesP will return endpoints instead of point and angle
	This allows us to more easily determine if an edge exists between two verticies

	Consider the following graph:

	O--O--O
	 \   /
	  \ /
	   O
	
	HoughLines will give only a point and an angle, therefore the algorithm will consider the top left 
	and top right directly connected even though they aren't
	However, HoughLinesP will return endpoints that likely lie within a reigon between the top left and top middle etc.
	
	*/

	//
	HoughLinesP(edges, u, 1, CV_PI / 180, 50, 50, 10);

	for (size_t i = 0; i < u.size(); i++) {

		Vec4i l = u[i];
		//Writes each detected line onto the image containing each vetrtex
		line(verticies, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);

	}
	//Shows the fully computer vision processed image
	imshow("Picture", verticies);
	Point v1;
	Point v2;

	/*
	
	In order to start assigning edges to particular verticies, a quadilateral reigon is created
	from each vertex to every other vertex which results in (v^2-v)/2 reigons.
	Each one of the 

	In each of the reigons every line will be inspected to see if both of its endpoints lie within
	the reigon and if the length of the line is 75% as long as the distance between the two verticies

	*/

	for (int i = 0; i < v.size(); i++) {
		for (int j = i + 1; i < v.size(); i++) {
			double theta;
			bool unsorted;

			if (v[i][0] == u[j][0]) {
				theta = -90;
				unsorted = false;
			}
			else if (v[i][0] < v[j][0] || !unsorted) {
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
	/*
	
	Assuming that the quadilateral is convex, determining 
	whether or not a point lies within it can be determined
	by the following algorithm:

	
	Draw a line from each of the 4 corners to the test point
	
	At this point there are 4 triangles

	Find the area of each of the 4 triangles and add them up.
		On a side note, there are two practical ways to find this area, Heron's formula
		or ABsin(c)/2 where sin(c) can be easily found using the dot product 
		definition

	If: the area of all 4 triangles adds up to the area of the quadilateral
		Then: the test point lies within the quadilateral
	Else: The point is outside the quadilateral

	NOTE: ONLY WORKS FOR CONVEX QUADILATERALS
	Extends to any convex polygon
	Divide figure figure into triangles for concave polygons 
	*/

	//Holds the lengths of every point-corner pair
	double lengths[4];

	//Assigns lengths[i] to the distance from corners[i] to the test point
	for (int i = 0; i < 4; i ++) {
		
		lengths[i] = dist(corners[i],test);
	}


	//Holds the sum of all of the areas of the side-test triangles
	double area = 0;

	//Takes area and adds onto it the area of each individual triangle
	for (int i = 0; i < 4; i ++) {

		//Heron's formula on lengths of each side of the triangle
		double sides[3] = { lengths[i], lengths[(i + 1) % 4],dist(corners[i],corners[(i + 1) % 4]) };
		
		//Adds on additional area
		area = area + trigArea(sides);
	}

	/*

	The area of a quadilateral can be found by dividing it into two triangles
	and adding the area of the two triangles

	*/


	//Holds the side lengths of the two triangles the quadilateral will be split into
	double quadLengths[2][3];
	
	//{{0,1},{1,2},{2,0}}
	//Use %3
	//{{0, 1 % 3},{1, 2 % 3},{2, 3 % 3}}
	for (int i = 0; i < 2; i++) {

		//Assigns quadLengths[i] to the distance between corners[i] and corners[i + 1]
		quadLengths[0][i] = dist(corners[i], corners[(i + 1)%3]);
	}

	//{{0,3},{3,2},{2,0}}
	//Use %4
	//{{4 % 4, 3 % 4},{3%4,2%4},{2%4,1%4}}
	//Last entry gives {2,1} instead of desired{2,0} so reassign
	for(int i = 4; i > 2; i--){

		//Array will write bacwards 2->1 but heron's formula isn't sensitive to order
		quadLengths[1][i-2] = dist(corners[i%4],corners[(i-1)%4]);
	}
	quadLengths[1][0] = dist(corners[2], corners[0]);

	//Compares the area of the quadilateral to the area of the triangles
	//Using sqrt leaves room for floating point and round error so use small epsilon
	if (abs(trigArea(quadLengths[1]) + trigArea(quadLengths[0]) - area) < .00001) {
		return true;
	}
	else {
		return false;
	}


}

double trigArea(double constraints[3], int method = HERON) {
	if (method == HERON) {
		//"https://en.wikipedia.org/wiki/Heron's_formula"
		double semisum = 0;

		for (int i = 1; i < 3; i++) {
			semisum = semisum + constraints[i] / 2;

		}

		double inside = semisum;

		for (int i = 1; i < 3; i++) {
			inside = inside * (semisum - constraints[i]);
		}

		return sqrt(inside);
	}
}

double dist(Point a, Point b) {
	//https://en.wikipedia.org/wiki/Pythagorean_theorem
	double x = double(a.x - b.x);
	x = x * x;
	double y = double(a.y - b.y);
	y = y * y;
	return sqrt(x+y);
}

double cosine(Point corner, Point topEdge, Point bottomEdge) {

}

int main(int argc, char** argv)
{


	return 1;
}