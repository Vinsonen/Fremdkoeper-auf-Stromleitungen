#pragma once
/*
* MyRoutines.h
*
*  Created on: 24.03.2014
*      Author: student
*/

#ifndef MYROUTINES_H_
#define MYROUTINES_H_
#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include<opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;
class MyRoutines {
public:
	MyRoutines();
	void CalculateHist(Mat image);
	int getkey();
	Mat farbeselect(Mat f1, Mat f2, Mat f3);
	Mat maskclock(Mat inpic, int schwelle);
	Mat findcenter(Mat inpic, Point* center, int* radius);
	void Uhrzeit(Point Center, Point zwoelf, Point kz, Point gz, int* Stunde, int* Minute,int* winke12);
	void findzwoelf(Mat inpic, Point* Center1);
	Mat findezeiger(Mat bild, Mat inpic, int radius, Point center, Point* kz, Point* gz);
	Mat maskalles(Mat inpic);
	Mat maskfremd(Mat inpic);
	Mat usefiltfindgrenze(Mat inpic );
	void drawgrenze(Mat inpic, Mat grenz);
	Mat stromleitunguseHoughLinesP(Mat inpic, Mat output);
	~MyRoutines();
};

#endif /* MYROUTINES_H_ */
