#include <iostream>
#include <vector>
#include <Dense>
#include <Core>
#include <opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include "ellipsesfitting.h"

using namespace cv;
using namespace std;
using namespace Eigen;

Mat mask1 = (Mat_<uchar>(3, 3) << 0, 255, 255, 0, 255, 255, 0, 255, 255);
Mat mask2 = (Mat_<uchar>(3, 3) << 255, 255, 0, 255, 255, 0, 255, 255, 0);
Mat mask3 = (Mat_<uchar>(3, 3) << 0, 0, 0, 255, 255, 255, 255, 255, 255);
Mat mask4 = (Mat_<uchar>(3, 3) << 255, 255, 255, 255, 255, 255, 0, 0, 0);
Mat mask5 = (Mat_<uchar>(3, 3) << 0, 0, 0, 0, 255, 0, 255, 255, 255);
Mat mask6 = (Mat_<uchar>(3, 3) << 255, 0, 255, 255, 255, 255, 255, 255, 255);
Mat mask7 = (Mat_<uchar>(3, 3) << 255, 255, 255, 255, 255, 255, 255, 0, 255);
Mat mask8 = (Mat_<uchar>(3, 3) << 255, 0, 0, 255, 255, 255, 255, 255, 255);
Mat mask9 = (Mat_<uchar>(3, 3) << 0, 0, 255, 255, 255, 255, 255, 255, 255);
Mat mask10 = (Mat_<uchar>(3, 3) << 255, 255, 255, 255, 255, 255, 0, 0, 255);
Mat mask11 = (Mat_<uchar>(3, 3) << 255, 255, 255, 255, 255, 255, 255, 0, 0);
Mat mask12 = (Mat_<uchar>(3, 3) << 255, 0, 0, 255, 255, 0, 255, 255, 255);
Mat mask13 = (Mat_<uchar>(3, 3) << 0, 0, 255, 0, 255, 255, 255, 255, 255);
Mat mask14 = (Mat_<uchar>(3, 3) << 255, 255, 255, 255, 255, 0, 255, 0, 0);
Mat mask15 = (Mat_<uchar>(3, 3) << 255, 255, 255, 0, 255, 255, 0, 0, 255);
Mat mask16 = (Mat_<uchar>(3, 3) << 255, 255, 255, 0, 255, 0, 0, 0, 0);
Mat mask17 = (Mat_<uchar>(3, 3) << 255, 0, 0, 255, 255, 0, 255, 0, 0);
Mat mask18 = (Mat_<uchar>(3, 3) << 0, 0, 255, 0, 255, 255, 0, 0, 255);
Mat mask19 = (Mat_<uchar>(3, 3) << 255, 0, 0, 255, 255, 0, 255, 255, 0);
Mat mask20 = (Mat_<uchar>(3, 3) << 0, 0, 255, 0, 255, 255, 0, 255, 255);
Mat mask21 = (Mat_<uchar>(3, 3) << 255, 255, 0, 255, 255, 0, 255, 0, 0);
Mat mask22 = (Mat_<uchar>(3, 3) << 0, 255, 255, 0, 255, 255, 0, 0, 255);
Mat mask23 = (Mat_<uchar>(3, 3) << 255, 255, 255, 0, 255, 255, 0, 255, 255);
Mat mask24 = (Mat_<uchar>(3, 3) << 255, 255, 255, 255, 255, 0, 255, 255, 0);
Mat mask25 = (Mat_<uchar>(3, 3) << 0, 255, 255, 0, 255, 255, 255, 255, 255);
Mat mask26 = (Mat_<uchar>(3, 3) << 255, 255, 0, 255, 255, 0, 255, 255, 255);
Mat mask27 = (Mat_<uchar>(3, 3) << 255, 255, 255, 0, 255, 255, 0, 0, 0);
Mat mask28 = (Mat_<uchar>(3, 3) << 255, 255, 255, 255, 255, 0, 0, 0, 0);
Mat mask29 = (Mat_<uchar>(3, 3) << 0, 0, 0, 0, 255, 255, 255, 255, 255);
Mat mask30 = (Mat_<uchar>(3, 3) << 0, 0, 0, 255, 255, 0, 255, 255, 255);
Mat mask31 = (Mat_<uchar>(3, 3) << 255, 255, 0, 255, 255, 0, 0, 0, 0);
Mat mask32 = (Mat_<uchar>(3, 3) << 0, 0, 0, 255, 255, 0, 255, 255, 0);
Mat mask33 = (Mat_<uchar>(3, 3) << 0, 255, 255, 0, 255, 255, 0, 0, 0);
Mat mask34 = (Mat_<uchar>(3, 3) << 0, 0, 0, 0, 255, 255, 0, 255, 255);

Mat BGR(Mat img);
Mat findPointMat(Point2f point, Mat img);
Point2f findNextPoint(int i, Point2f lastpoint);
vector<Point2f> collectBoundary(Mat img, Mat& collect, vector<Mat> mask);
Mat performClosing(Mat img, int elem, int size);
bool isEqual(Mat a, Mat b);
vector<Mat> loadMask(void);

int main(int argc, char** argv)
{
	vector<Mat> mask;
	mask = loadMask();
	Mat oriimg = imread("D:/project/c++project/LSFOE/LSFOE/src/1.jpg");
	Mat out;
	out = BGR(oriimg);
	Mat collectimg = Mat::zeros(Size(oriimg.cols, oriimg.rows), CV_8UC1);
	vector<Point2f> pf2;
	pf2 = collectBoundary(out, collectimg, mask);

	Ellipses ellipses;
	ellipses.GetBoundaryPoints(pf2);
	ellipses.BuildUnknownNumberMat();
	ellipses.SolveEllipses();

	ellippara ep;

	ep = ellipses.FittingSolution();

	cout << ep.xc << endl;
	cout << ep.yc << endl;
	cout << ep.ma << endl;
	cout << ep.sma << endl;
	cout << ep.theta << endl;

	ellipse(oriimg, Point(ep.yc, ep.xc), Size(ep.ma, ep.sma), ep.theta, 0, 360, Scalar(255, 0, 0), 2, 8);
	circle(oriimg, Point(ep.yc, ep.xc), 3, Scalar(255, 0, 0), -1);

	imshow("coll", collectimg);
	imshow("ori", oriimg);
	imshow("out", out);
	

	waitKey(0);

	return 0;
}

Mat BGR(Mat img)
{
	Mat output = Mat::zeros(Size(img.cols, img.rows), CV_8UC1);

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (2 * (img.at<Vec3b>(i, j)[2] - 2 * img.at<Vec3b>(i, j)[1]) > 100)
			{
				output.at<uchar>(i, j) = 255;
			}
		}
	}

	Mat noise;
	medianBlur(img, noise, 3);

	output = performClosing(output, MORPH_ELLIPSE, 3);

	return output;

}

vector<Point2f> collectBoundary(Mat img, Mat& collect, vector<Mat> mask)
{
	bool flag = false;
	int num = 0;
	vector<Point2f> pfbc;
	Point2f start;
	Point2f now;
	Point2f next;
	Mat pointmat;

	for (int i = 0; i < img.cols; i++)
	{
		for (int j = 0; j < img.rows; j++)
		{
			if (img.at<uchar>(j, i) == 255)
			{
				start = Point2f(j, i);
				now = start;
				next = now;
				flag = true;
				break;
			}
		}
		if (flag)break;
	}

	pfbc.push_back(now);
	pointmat = findPointMat(now, img);
	collect.at<uchar>(now.x, now.y) = 255;

	for (int i = 0; i < mask.size(); i++)
	{
		if (isEqual(pointmat, mask[i]))
		{
			next = findNextPoint(i, now);
			pfbc.push_back(next);
			now = next;
			collect.at<uchar>(now.x, now.y) = 255;
			break;
		}
	}

	while (now != start)
	{
		num++;
		pointmat = findPointMat(now, img);
		for (int i = 0; i < mask.size(); i++)
		{
			if (isEqual(pointmat, mask[i]))
			{
				next = findNextPoint(i, now);
				pfbc.push_back(next);
				now = next;
				collect.at<uchar>(now.x, now.y) = 255;
				break;
			}
		}
	}

	for (int i = 0; i < pfbc.size(); i++)
	{
		cout << pfbc[i] << endl;
	}

	return pfbc;
}

Mat performClosing(Mat img, int elem, int size)
{
	Mat output, temp;
	Mat element = getStructuringElement(elem, Size(2 * size + 1, 2 * size + 1), Point(size, size));

	dilate(img, temp, element);
	erode(temp, output, element);

	return output;
}

bool isEqual(Mat a, Mat b)
{
	Mat diff = a != b;
	bool eq = countNonZero(diff) == 0;
	return eq;
}

vector<Mat> loadMask(void)
{
	vector<Mat> maskback;
	maskback.push_back(mask1);
	maskback.push_back(mask2);
	maskback.push_back(mask3);
	maskback.push_back(mask4);
	maskback.push_back(mask5);
	maskback.push_back(mask6);
	maskback.push_back(mask7);
	maskback.push_back(mask8);
	maskback.push_back(mask9);
	maskback.push_back(mask10);
	maskback.push_back(mask11);
	maskback.push_back(mask12);
	maskback.push_back(mask13);
	maskback.push_back(mask14);
	maskback.push_back(mask15);
	maskback.push_back(mask16);
	maskback.push_back(mask17);
	maskback.push_back(mask18);
	maskback.push_back(mask19);
	maskback.push_back(mask20);
	maskback.push_back(mask21);
	maskback.push_back(mask22);
	maskback.push_back(mask23);
	maskback.push_back(mask24);
	maskback.push_back(mask25);
	maskback.push_back(mask26);
	maskback.push_back(mask27);
	maskback.push_back(mask28);
	maskback.push_back(mask29);
	maskback.push_back(mask30);
	maskback.push_back(mask31);
	maskback.push_back(mask32);
	maskback.push_back(mask33);
	maskback.push_back(mask34);

	return maskback;
}

Mat findPointMat(Point2f point, Mat img)
{
	Mat PointMat = (Mat_<uchar>(3, 3) << 0, 0, 0, 0, 0, 0, 0, 0, 0);
	PointMat.at<uchar>(0, 0) = img.at<uchar>(point.x - 1, point.y - 1);
	PointMat.at<uchar>(0, 1) = img.at<uchar>(point.x - 1, point.y);
	PointMat.at<uchar>(0, 2) = img.at<uchar>(point.x - 1, point.y + 1);
	PointMat.at<uchar>(1, 0) = img.at<uchar>(point.x, point.y - 1);
	PointMat.at<uchar>(1, 1) = img.at<uchar>(point.x, point.y);
	PointMat.at<uchar>(1, 2) = img.at<uchar>(point.x, point.y + 1);
	PointMat.at<uchar>(2, 0) = img.at<uchar>(point.x + 1, point.y - 1);
	PointMat.at<uchar>(2, 1) = img.at<uchar>(point.x + 1, point.y);
	PointMat.at<uchar>(2, 2) = img.at<uchar>(point.x + 1, point.y + 1);

	return PointMat;
}

Point2f findNextPoint(int i, Point2f lastpoint)
{
	Point2f pointback;
	switch (i)
	{
	case 0:pointback = Point2f(lastpoint.x + 1, lastpoint.y); break;
	case 1:pointback = Point2f(lastpoint.x - 1, lastpoint.y); break;
	case 2:pointback = Point2f(lastpoint.x, lastpoint.y - 1); break;
	case 3:pointback = Point2f(lastpoint.x, lastpoint.y + 1); break;
	case 4:pointback = Point2f(lastpoint.x + 1, lastpoint.y - 1); break;
	case 5:pointback = Point2f(lastpoint.x - 1, lastpoint.y - 1); break;
	case 6:pointback = Point2f(lastpoint.x + 1, lastpoint.y + 1); break;
	case 7:pointback = Point2f(lastpoint.x - 1, lastpoint.y - 1); break;
	case 8:pointback = Point2f(lastpoint.x, lastpoint.y - 1); break;
	case 9:pointback = Point2f(lastpoint.x + 1, lastpoint.y + 1); break;
	case 10:pointback = Point2f(lastpoint.x, lastpoint.y + 1); break;
	case 11:pointback = Point2f(lastpoint.x - 1, lastpoint.y - 1); break;
	case 12:pointback = Point2f(lastpoint.x + 1, lastpoint.y - 1); break;
	case 13:pointback = Point2f(lastpoint.x - 1, lastpoint.y + 1); break;
	case 14:pointback = Point2f(lastpoint.x + 1, lastpoint.y + 1); break;
	case 15:pointback = Point2f(lastpoint.x - 1, lastpoint.y + 1); break;
	case 16:pointback = Point2f(lastpoint.x - 1, lastpoint.y - 1); break;
	case 17:pointback = Point2f(lastpoint.x + 1, lastpoint.y + 1); break;
	case 18:pointback = Point2f(lastpoint.x - 1, lastpoint.y - 1); break;
	case 19:pointback = Point2f(lastpoint.x + 1, lastpoint.y); break;
	case 20:pointback = Point2f(lastpoint.x - 1, lastpoint.y); break;
	case 21:pointback = Point2f(lastpoint.x + 1, lastpoint.y + 1); break;
	case 22:pointback = Point2f(lastpoint.x + 1, lastpoint.y); break;
	case 23:pointback = Point2f(lastpoint.x - 1, lastpoint.y + 1); break;
	case 24:pointback = Point2f(lastpoint.x + 1, lastpoint.y - 1); break;
	case 25:pointback = Point2f(lastpoint.x - 1, lastpoint.y); break;
	case 26:pointback = Point2f(lastpoint.x, lastpoint.y + 1); break;
	case 27:pointback = Point2f(lastpoint.x - 1, lastpoint.y + 1); break;
	case 28:pointback = Point2f(lastpoint.x + 1, lastpoint.y - 1); break;
	case 29:pointback = Point2f(lastpoint.x, lastpoint.y - 1); break;
	case 30:pointback = Point2f(lastpoint.x - 1, lastpoint.y); break;
	case 31:pointback = Point2f(lastpoint.x, lastpoint.y - 1); break;
	case 32:pointback = Point2f(lastpoint.x, lastpoint.y + 1); break;
	case 33:pointback = Point2f(lastpoint.x + 1, lastpoint.y); break;
	}

	return pointback;
}
