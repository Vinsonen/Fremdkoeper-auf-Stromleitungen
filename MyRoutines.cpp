#include <iostream>
#include <strstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "MyRoutines.h"
using namespace cv;
using namespace std;
MyRoutines::MyRoutines() {
	// TODO Auto-generated constructor stub

}

MyRoutines::~MyRoutines() {
	// TODO Auto-generated destructor stub
}
void MyRoutines::CalculateHist(Mat image) {
	int histSize = 256;
	Mat dst, hist;
	namedWindow("histogram", 0);
	image.convertTo(dst, CV_8U);
	calcHist(&dst, 1, 0, Mat(), hist, 1, &histSize, 0);
	Mat histImage = Mat::ones(200, 512, CV_8U) * 255;
	normalize(hist, hist, 0, histImage.rows, CV_MINMAX, CV_32F);
	histImage = Scalar::all(255);
	int binW = cvRound((double)histImage.cols / histSize);
	for (int i = 0; i < histSize; i++)
		rectangle(histImage, Point(i*binW, histImage.rows),
			Point((i + 1)*binW, histImage.rows - cvRound(hist.at<float>(i))),
			Scalar::all(0), -1, 8, 0);
	imshow("histogram", histImage);
}
   Mat MyRoutines::farbeselect(Mat f1, Mat f2,Mat f3)
{
	return (2* f1 - f2 - f3);
}
   Mat  MyRoutines::maskalles(Mat inpic) {
	   dilate(inpic, inpic, Mat(), Point(-1, -1), 4);
	   erode(inpic, inpic, Mat(), Point(-1, -1), 4);

	   dilate(inpic, inpic, Mat(), Point(-1, -1), 1);
	   return inpic;
   }
   Mat  MyRoutines::maskfremd(Mat inpic) {
	   //通过binaer之后的先放大 后缩小即电线vvv  反则为异物  异物还需要dilate
	   erode(inpic, inpic, Mat(), Point(-1, -1), 2);
	   dilate(inpic, inpic, Mat(), Point(-1, -1), 10);
	  // dilate(inpic, inpic, Mat(), Point(-1, -1), 12);
	  // erode(inpic, inpic, Mat(), Point(-1, -1), 4);	//异物还需要dilate  ***参数可调圆圈大小
	   //异物还需要dilate
	   return inpic;
   }


   // HoughLines   画出图像中的电线
   Mat MyRoutines::stromleitunguseHoughLinesP(Mat inpic,Mat output) {
	   Mat dst,color_dst;
	   Canny(inpic, dst, 50, 200, 3);		//**********处理边界最理想
	   cvtColor(dst, color_dst, CV_GRAY2BGR);

	   vector<Vec4i> lines;
	   //*********
	   HoughLinesP(dst, lines, 1, CV_PI / 180, 80, 30, 10);
	   for (size_t i = 0; i < lines.size(); i++)
	   {
		   line(output, Point(lines[i][0], lines[i][1]),
			   Point(lines[i][2], lines[i][3]), Scalar(0, 0, 255), 3, 8);//通过点的连接形成线
	   }

	   return color_dst;
   }

   void  MyRoutines::drawgrenze(Mat inpic,Mat grenz){
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;

			findContours(grenz, contours, hierarchy,
				CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); //	画出边缘
			int idx = 0;
			for (; idx >= 0; idx = hierarchy[idx][0])
			{
			
				Scalar color(255,0,255);
				drawContours(inpic, contours, idx, color, CV_FILLED, 8, hierarchy,5);
			}
   }
   Mat  MyRoutines::usefiltfindgrenze(Mat inpic) {
	   //输入为binaer输出边缘
	   Mat inpicrow, inpiccol;
	   float kdata1[] = { -1,0,-1,
					    -1,0,-1,
					     -1,0,-1 };
	   Mat kernel0(3, 3, CV_32F, kdata1);
	   filter2D(inpic, inpicrow, -1, kernel0);

	   float kdata2[] = { -1,-1,-1,
							0,0,0,
							-1,-1,-1
	   };
	   Mat kernel2(3, 3, CV_32F, kdata2);
	   filter2D(inpic, inpiccol, -1, kernel2);



	   add(inpicrow, inpiccol,inpic);
	   return inpic;
   }

   Mat  MyRoutines::maskclock(Mat inpic, int schwelle) 
   {
	   Mat bild;
		threshold(inpic, bild, schwelle, 255, THRESH_BINARY);//取出图像为binaer
		erode(bild, bild, Mat(), Point(-1, -1), 3);
		dilate(bild, bild, Mat(), Point(-1, -1), 3);
		dilate(bild, bild, Mat(), Point(-1, -1), 12);
		erode(bild, bild, Mat(), Point(-1, -1), 12);
		return bild;
   }
   Mat MyRoutines::findcenter(Mat inpic, Point * Center, int* Radius) 
   {
	   medianBlur(inpic, inpic, 5);
	   GaussianBlur(inpic, inpic, Size(9, 9), 2, 2);
	   vector<Vec3f>circles;
	   HoughCircles(inpic, circles, HOUGH_GRADIENT,
		             1, inpic.rows / 6, 100, 30, 1, inpic.cols / 2);
	   for (size_t i = 0; i < circles.size(); i++)
	   {
		   Vec3i c = circles[i];
		   Point  center = Point(c[0], c[1]);
		   // circle center
		   circle(inpic,  center, 1, Scalar(0, 100, 100),5, LINE_AA);
		   // circle outline
		   int radius = c[2];
		   circle(inpic,  center, radius, Scalar(255, 0, 255), 8, LINE_AA);

		   * Center = center;
		   *Radius = radius;
	   }	 
	   return inpic;
   }
   //void MyRoutines::findzwoelf(Mat inpic, Point* Center1) 
   //{   
	  // Point zwoelf;
	  // threshold(inpic, inpic, 50, 255, THRESH_BINARY);
	  // Moments m = moments(inpic);
	  // zwoelf.x = m.m10 / m.m00;
	  // zwoelf.y = m.m01 / m.m00;
	  // *Center1 = zwoelf;
	  // //circle(inpic, zwoelf, 10, CV_RGB(255, 255, 0), CV_FILLED, LINE_AA);
   //}

   //Mat MyRoutines::findezeiger(Mat bild, Mat inpic, int radius, Point center, Point* gz, Point* kz)
   //{
	  // int i = 0;
	  // unsigned char colorc, colord;
	  // for (i; i < 360; i = i + 1)
	  // {
		 //  int r = radius;
		 //  Point ca = { center.x - int(1.5 * r * cos(i * 3.1415 / 180)),
			//		   center.y - int(1.5 * r * sin(i * 3.1415 / 180)) };
		 //  colorc = inpic.at<unsigned char>(ca);          //对。。作对比
		 //  if (colorc) 
		 //  {
			//   circle(bild, ca, 5, CV_RGB(255, 0, 0), CV_FILLED, LINE_AA);
			//   *gz = ca;
			//   break;
		 //  }
	  // }
	  // for (i; i > -360; i = i - 1)
	  // {
		 //  i -= 9;
		 //  int r = radius;
		 //  Point da = { center.x - int(0.5 * r * cos(i * 3.1415 / 180)),
			//	   center.y - int(0.5 * r * sin(i * 3.1415 / 180)) };
		 //  colord = inpic.at<unsigned char>(da);
		 //  if (colord)
		 //  {
			//   circle(bild, da, 5, CV_RGB(0, 255, 0), CV_FILLED, LINE_AA);
			//   *kz = da;
			//   break;
		 //  }
	  // }
	  // return bild;
   //}


   void MyRoutines::Uhrzeit(Point Center, Point zwoelf, Point da, Point ca, int* Stunde, int* Minute,int * Winke12) {
	   Point wkz, wgz, w12;
	   wkz = { (da.x - Center.x),(da.y - Center.y) };
	   wgz = { (ca.x - Center.x),(ca.y - Center.y) };
	   w12 = { zwoelf.x - Center.x,zwoelf.y - Center.y };
	   double winkel12 = cvFastArctan(w12.x, w12.y);

	   double winkelk = cvFastArctan(wkz.x, (1) * wkz.y);
	   double winkelg = cvFastArctan(wgz.x, (1) * wgz.y);   
	   winkelk -= winkel12;winkelg -= winkel12;
	   while (winkelk > 360 || winkelk < 0) {
		   if (winkelk < 0) winkelk += 360; if (winkelk >360 ) winkelk -= 360;
	   }
	   while (winkelg > 360 || winkelg < 0) {
		  if (winkelg < 0) winkelg += 360; if (winkelg > 360) winkelg -= 360;
	   }
	   while (winkel12 > 360 || winkel12 < 0) {
		   if (winkel12 < 0) winkel12 += 360; if (winkel12 > 360) winkel12 -= 360;
	   }
	 //  winkelk /= 30;winkelg /= 6;
	   *Stunde = (360- winkelk)/30 ;
	   *Minute = (360- winkelg)/6 ;
	   *Winke12 = winkel12;
	  
   }
 /*void  MyRoutines::Uhrzeit(Point Center, Point zwoelf, Point kz, Point gz, int* Stunde, int* Minute) {
	   Point zwoelf1 = zwoelf - Center;
	   Point kzl = kz - Center;
	   Point gzl = gz - Center;
	   double w12 = cvFastArctan(zwoelf1.y, zwoelf1.x);
	   double wkz = cvFastArctan(kzl.y, kzl.x);
	   double wgz = cvFastArctan(gzl.y, gzl.x);

	   double wskz = wkz - w12; if (wskz < 0)wskz += 360;
	   double wmgz = wgz - w12; if (wmgz < 0)wmgz += 360;
	   *Stunde = wskz / 30;
	   *Minute = wmgz / 6;
   }*/
   

int MyRoutines::getkey() {
	return (cvWaitKey(0));
}
