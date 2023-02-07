#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <strstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "MyRoutines.h"

using namespace cv;
using namespace std;

// 说明 一般的图像 都会有蓝天 绿树  黑线 异物
//先缩小再放大

int main(int argc, char** argv)
{
	int radius;
	Mat image, Filter,tmpMat, image2grayGrau, hist_image, tred, tgreen, tblue, dst, color_dst,stromleitung,
		tmerge, tmerge1, nurrot, nurblau, nurgruen, image2gray,binar;
	Point center,center1,kz,gz,ca,da;
	MyRoutines Werkzeug;
	Moments m;
	vector <Mat> rgb;

	  int b,c,d,e = 0;
	  string win1 = "output";
	  namedWindow(win1, WINDOW_NORMAL);
	 // namedWindow("nurblau", WINDOW_AUTOSIZE);
	 // namedWindow("nurgruen", WINDOW_AUTOSIZE);
	 // namedWindow("nurrot", WINDOW_AUTOSIZE);
	  namedWindow("Merge", WINDOW_AUTOSIZE);
	  namedWindow("image", WINDOW_AUTOSIZE);
	  namedWindow("stromleitung", WINDOW_NORMAL);
	  namedWindow("Grau", WINDOW_NORMAL);
	  namedWindow("Binar", WINDOW_NORMAL);


	  createTrackbar("graygrenz", win1, &b, 255, 0);
	  createTrackbar("2graygrenz", "nurblau", &c, 255, 0);
	  createTrackbar("2graygrenz", "nurrot", &d, 255, 0);
	  createTrackbar("2graygrenz", "nurgruen", &e, 255, 0);

		for (;;) {
			string inputbild = "777.jpg";  //p1,p2 鸟窝 冰雪
			image = imread(inputbild, CV_LOAD_IMAGE_COLOR);    //导入外接图片f
			stromleitung= imread(inputbild, CV_LOAD_IMAGE_COLOR);
			Filter = imread(inputbild, CV_LOAD_IMAGE_COLOR);

			if (!image.data)                              // Check for invalid input
			{
				cout << "Could not open or find the image" << std::endl;
				return -1;
			}
			int size = 2.5;
			resize(Filter, Filter, Size(), size, size, CV_INTER_AREA);
			resize(image, image, Size(), size, size, CV_INTER_AREA);
			resize(stromleitung, stromleitung, Size(), size, size, CV_INTER_AREA);

		
			float kdata[] = { -1,-1,-1,// shi  Filter 
							0,0,0,
							  1,1,1 };
			Mat kernel(3, 3, CV_32F, kdata);
			filter2D(Filter, tmpMat, -1, kernel);

							split(image, rgb);
							tblue = rgb[0];
							tgreen = rgb[1];
							tred = rgb[2];
							nurgruen = Werkzeug.farbeselect(tgreen, tblue, tred);
							nurblau = Werkzeug.farbeselect(tblue, tred, tgreen);
							nurrot = Werkzeug.farbeselect(tred, tblue, tgreen);

							nurblau = Werkzeug.maskclock(nurblau, c);
							nurrot = Werkzeug.maskclock(nurrot, d);
							nurgruen = Werkzeug.maskclock(nurgruen, e);

			cvtColor(image, image2gray, CV_BGR2GRAY);
			cvtColor(image, image2grayGrau, CV_BGR2GRAY);
			threshold(image2gray, image2gray, 109, 255, THRESH_BINARY_INV);//黑白图片chu image
		 threshold(image2grayGrau, binar, 109, 255, THRESH_BINARY_INV);//binar测试

			//nurgruen = Werkzeug.findcenter(nurgruen, &center, &radius);
			////通过binaer之后的先放大 后缩小即电线vvv  反则为异物  异物还需要dilate
		 //       erode(image, image, Mat(), Point(-1, -1), 4);			
			//dilate(image, image, Mat(), Point(-1, -1), 4);
			//dilate(image, image, Mat(), Point(-1, -1), 12);
			//erode(image, image, Mat(), Point(-1, -1), 8);
			////异物还需要dilate、
			
	//选择是识别电线加异物和仅仅是异物
			//image2gray=Werkzeug.maskalles(image2gray);
			image2gray = Werkzeug.maskfremd(image2gray);

		    Canny(image2gray,image2gray,50,150);	//边缘算法  取出异物的边缘圈出

			dilate(image2gray, image2gray, Mat(), Point(-1, -1), 2);

			Werkzeug.drawgrenze(image, image2gray);

			
	
			color_dst=Werkzeug.stromleitunguseHoughLinesP(image,stromleitung);
						bitwise_and(tred, image2gray, tred);
						bitwise_and(tblue, image2gray, tblue);
						bitwise_and(tgreen, image2gray, tgreen);
							merge(rgb, tmerge);
	
			imshow("stromleitungbinar", color_dst);
			imshow("stromleitung", stromleitung);
			imshow(win1, image2gray);
			imshow("image", image);
			imshow("Grau", image2grayGrau);
			imshow("Binar", binar);
			//imshow("nurblau", nurblau);
			//imshow("nurgruen", nurgruen);
			//imshow("nurrot", nurrot);
			imshow("Merge", tmerge);

	
			char c;
			c = waitKey(0);
			if (c == 's') imwrite("result.jpg", image);
			if (Werkzeug.getkey() == 27)  //Teste auf ESC
			{		      
				cout << "tschuess ! " << std::endl;
				waitKey(0);										  // Warte auf Tastendruck
			
				break; //ESC
			}
		}		
		destroyAllWindows();
		return 0;
}


