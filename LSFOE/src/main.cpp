#include <iostream>
#include <vector>
#include <Dense>
#include <Core>
#include <opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "ellipsesfitting.h"
#include "binocularcamera.h"
#include "prefittingclassify.h"
#include "serial.h"

using namespace cv;
using namespace std;
using namespace Eigen;

bool scanning(Mat oriimg);

int main(int argc, char** argv)
{
	Mat src1, gray_src, src2, dst;
	src1 = imread("D:/project/c++project/LSFOE/LSFOE/src/1.png");

	if (src1.empty())

	{

		cout << "could not find src1" << endl;

		return -1;

	}

	//namedWindow("input", CV_WINDOW_AUTOSIZE);

	//imshow("input", src1);

	//»ô·òÔ²¼ì²â   £¨ÖÐÖµÈ¥½·ÑÎ£©ÂË²¨ --> cvtcolor×ª»Ò¶È --> Houghcircle±ßÔµ¼ì²â

	medianBlur(src1, src2, 3);

	cvtColor(src2, gray_src, COLOR_BGR2GRAY);

	vector<Vec3f>pcircles;

	HoughCircles(gray_src, pcircles, HOUGH_GRADIENT, 2, 100, 150, 200, 0, 1000);

	src1.copyTo(dst);

	for (size_t i = 0; i < pcircles.size(); i++)

	{

		Vec3f circles = pcircles[i];

		circle(dst, Point(circles[0], circles[1]), circles[2], Scalar(255, 0, 0), 1, LINE_AA);

	}

	imshow("output", dst);

	waitKey(0);

	return 0;
	//Serial uart;
	//uart.open("COM5", 115200);

	//uart.send(1, 1500, 2000);
	//Sleep(2000);
	////while (!uart.receive(1));
	//uart.send(2, 1500, 2000);
	//Sleep(2000);
	////while (!uart.receive(1));
	//uart.send(3, 1500, 2000);
	//Sleep(2000);
	////while (!uart.receive(1));
	////while (!uart.receive(1));
	//uart.send(5, 1500, 2000);
	//Sleep(2000);
	////while (!uart.receive(1));
	//uart.send(6, 1500, 2000);
	//Sleep(2000);
	////while (!uart.receive(1));
	//uart.send(4, 1833, 2000);
	//Sleep(2000);
	//cout << "get callback" << endl;
	//int xval = 1500;
	//int yval = 1833;
	//bool scanflag = true;
	//int scancount = 0;
	//int scansequence[3] = { 853,1500,2206 };
	//int key = 0;
	//int flag=0;
	//string imgnamel, imgnamer;
	//Mat ori1, ori2, out, aim;
	//int f = 1;
	//int z = 1;
	//Camera cam;
	//cam.Open(1);
	//angle myangle;
	//Ellipses ellipses;
	//vector<Point2f> pf2;
	//vector<Mat> mask;
	//mask = loadMask();
	//int id[3] = { 2,3,4 };
	//int num[3];

	//uart.send(2, 1500, 1000);
	////while (!uart.receive(1));
	//uart.send(3, 1500, 1000);
	////while (!uart.receive(1));
	//uart.send(4, 1500, 1000);
	////while (!uart.receive(1));
	
	/*uart.send(2, myangle.th1, 1000);
	uart.send(3, myangle.th2, 1000);
	uart.send(4, myangle.th3, 1000);*/

	//while (1)
	//{
	//	ori1 = cam.GetLeftimg(CAMERA_SHOWN);
	//	ori2 = cam.GetRightimg(CAMERA_SHOWN);

	//	//if (!scanning(ori1))
	//	//{
	//	//	uart.send(1, scansequence[scancount], 2000);
	//	//	while (!uart.receive(1));
	//	//	if (scancount == 0)scanflag = true;
	//	//	if (scancount == 2)scanflag = false;
	//	//	if (scanflag)scancount++;
	//	//	else scancount--;
	//	//}
	//	//else
	//	//{
	//		if (key == 'w') flag = 1;
	//		if (key == 's') flag = 0;
	//		if (flag == 1)cam.Zoom(ZOOM_PLUS);
	//		if (flag == 0)cam.Zoom(ZOOM_OUT);
	//		if (key == 'q') break;
	//		out = BGR(ori1);
	//		aim = out;
	//		if (ROIlocate(out, xval, yval, uart))
	//		{
	//			flag = 3;
	//			ori1 = cam.GetLeftimg(CAMERA_SHOWN);
	//			ori2 = cam.GetRightimg(CAMERA_SHOWN);
	//			cout << "got it" << endl;
	//		}

	//		for (int i = 0; i < ori1.rows; i++)
	//		{
	//			for (int j = 0; j < ori1.cols; j++)
	//			{
	//				if (i == 240)aim.at<uchar>(i, j) = 255;
	//				if (j == 400)aim.at<uchar>(i, j) = 255;
	//			}
	//		}

	//		imshow("aim", aim);

	//		if (key == 'l')flag = 3;

	//		if (flag == 3)
	//		{
	//			Mat aim = Mat::zeros(Size(ori1.size()), CV_8UC1);
	//			out = BGR(ori1);
	//			pf2 = collectBoundary(out, mask);
	//			ellipses.GetBoundaryPoints(pf2);
	//			ellipses.BuildUnknownNumberMat();
	//			ellipses.SolveEllipses();

	//			ellippara ep;

	//			ep = ellipses.FittingSolution();

	//			cam.Stereocorrection();
	//			cam.SGBM();
	//			cam.Get3Dpos(ep.xc, ep.yc, yval, myangle);

	//			ellipse(ori1, Point(ep.yc, ep.xc), Size(ep.ma, ep.sma), ep.theta, 0, 360, Scalar(255, 0, 0), 2, 8);
	//			imshow("out", ori1);
	//			flag = 0;
	//			break;
	//		}

	//		//if (flag == 3) 
	//		//{
	//		//	out = BGR(ori);
	//		//	pf2 = collectBoundary(out, mask);
	//		//	ellipses.GetBoundaryPoints(pf2);
	//		//	ellipses.BuildUnknownNumberMat();
	//		//	ellipses.SolveEllipses();

	//		//	ellippara ep;

	//		//	ep = ellipses.FittingSolution();

	//		//	ellipse(ori, Point(ep.yc, ep.xc), Size(ep.ma, ep.sma), ep.theta, 0, 360, Scalar(255, 0, 0), 2, 8);
	//		//	imshow("out", ori);
	//		//}

	//		/*out = BGR(ori);
	//		imshow("out", out);*/
	//	//}
	//	key = cv::waitKey(30);
	//}
	//	
	//cv::waitKey(0);

	////num[0] = myangle.th1;
	////num[1] = myangle.th2;
	////num[2] = myangle.th3;

	////uart.send(6, 1118, 1000);
	////
	/////*uart.send(4, myangle.th3, 1500);
	////while (!uart.receive(1));
	////uart.send(3, myangle.th2, 1500);
	////while (!uart.receive(1));
	////uart.send(2, myangle.th1, 1500);
	////while (!uart.receive(1));*/
	////uart.sendx(3, id, num, 1500);
	////Sleep(1500);
	////uart.send(6, 1600, 1000);
	////Sleep(1000);
	/////*uart.send(2, 1500, 2000);
	////while (!uart.receive(1));
	////uart.send(3, 1500, 2000);
	////while (!uart.receive(1));
	////uart.send(4, 1500, 2000);
	////while (!uart.receive(1));*/
	////num[0] = 1500;
	////num[1] = 1500;
	////num[2] = 1500;
	////uart.sendx(3, id, num, 1500);
	////Sleep(1500);

	return 0;
}

bool scanning(Mat oriimg)
{
	bool hit = false;
	Mat out = BGR(oriimg);
	for (int i = 0; i < out.rows; i++)
	{
		for (int j = 0; j < out.cols; j++)
		{
			if (out.at<uchar>(i, j) == 255)hit = true;
		}
	}
	return hit;
}

