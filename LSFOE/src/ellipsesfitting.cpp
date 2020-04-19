#include "ellipsesfitting.h"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;
using namespace Eigen;

Ellipses::Ellipses()
{
	H = Mat::zeros(6, 6, CV_64F);
	H.at<double>(0, 2) = 2;
	H.at<double>(2, 0) = 2;
	H.at<double>(1, 1) = -1;
}

Ellipses::~Ellipses()
{
	H.release();
	X.release();
	W.release();
	S.release();
}

void Ellipses::GetBoundaryPoints(vector<Point2f> boundarypoints)
{
	bp = boundarypoints;
}

void Ellipses::BuildUnknownNumberMat()
{
	X = Mat::ones(6, bp.size(), CV_64F);
	for (int i = 0; i < bp.size(); i++)
	{
		X.at<double>(0, i) = pow(bp[i].x, 2);
		X.at<double>(1, i) = bp[i].x * bp[i].y;
		X.at<double>(2, i) = pow(bp[i].y, 2);
		X.at<double>(3, i) = bp[i].x;
		X.at<double>(4, i) = bp[i].y;
	}
}

void Ellipses::SolveEllipses()
{
	Mat XT = X.t();
	S = X * XT;
	Mat S_ = S.inv();
	Mat M = S_ * H;
	Mat L, V;
	eig(M, L, V);
	W = Mat::zeros(6, 1, CV_64F);
	Mat WT, Delta;
	double lambda = 0;
	double FA1 = 0;
	double FA2 = 0;
	double alpha = 0;
	double tag = 0;
	double Ddata = 0;
	double Ldata = 0;

	for (int i = 0; i < 6; i++)
	{
		Ldata = L.at<double>(i, i);
		if (L.at<double>(i, i) <= 0)continue;
		for (int j = 0; j < 6; j++)
		{
			W.at<double>(j, 0) = V.at<double>(j, i);
		}
		WT = W.t();
		Delta = WT * H * W;
		Ddata = Delta.at<double>(0, 0);
		if (Delta.at<double>(0, 0) < 0)continue;
		Delta.at<double>(0, 0) = 1 / Delta.at<double>(0, 0);
		lambda = sqrt(Delta.at<double>(0, 0));
		W = lambda * W;
		break;
	}
	A = W.at<double>(0, 0);
	B = W.at<double>(1, 0);
	C = W.at<double>(2, 0);
	D = W.at<double>(3, 0);
	E = W.at<double>(4, 0);
	F = W.at<double>(5, 0);

	ellipseparameters.xc = (B * E - 2 * C * D) / (4 * A * C - pow(B, 2));
	ellipseparameters.yc = (B * D - 2 * A * E) / (4 * A * C - pow(B, 2));

	FA1 = 2 * (A * pow(ellipseparameters.xc, 2) + C * pow(ellipseparameters.yc, 2) +
		B * ellipseparameters.xc * ellipseparameters.yc - F);
	FA2 = sqrt(pow((A - C), 2) + pow(B, 2));

	ellipseparameters.ma = sqrt(FA1 / (A + C + FA2));
	if (A + C - FA2 != 0)ellipseparameters.sma = sqrt(FA1 / (A + C - FA2));
	else ellipseparameters.sma = 0;

	if (B == 0 && F * A < F * C)ellipseparameters.theta = 0;
	if (B == 0 && F * A >= F * C)ellipseparameters.theta = 90;
	if (B != 0 && F * A < F * C)
	{
		alpha = atan((A - C) / B) * 180 / PI;
		if (alpha > 0)ellipseparameters.theta = 0.5 * (90 + alpha);
		else ellipseparameters.theta = 0.5 * (alpha - 90);
	}
	if (B != 0 && F * A >= F * C)
	{
		alpha = atan((A - C) / B) * 180 / PI;
		if (alpha > 0)ellipseparameters.theta = 90 + 0.5 * (90 + alpha);
		else ellipseparameters.theta = 90 + 0.5 * (alpha - 90);
	}

	if (ellipseparameters.ma < ellipseparameters.sma)
	{
		tag = ellipseparameters.ma;
		ellipseparameters.ma = ellipseparameters.sma;
		ellipseparameters.sma = tag;
	}
}

ellippara Ellipses::FittingSolution()
{
	return ellipseparameters;
}

Mat EigenVSort(Mat V)
{
	Mat newV;
	newV = V.clone();
	vector<double> stack;
	for (int i = 0; i < V.rows; i++)
	{
		for (int j = 0; j < V.cols; j++)
		{
			stack.push_back(V.at<double>(i, j));
		}
	}

	for (int j = 0; j < V.cols; j++)
	{
		for (int i = V.rows - 1; i >= 0; i--)
		{
			newV.at<double>(i, j) = stack.back();
			stack.pop_back();
		}
	}
	return newV;
}

void eig(cv::Mat A, cv::Mat& L, cv::Mat& V)
{
	MatrixXd mata(A.rows, A.cols);
	MatrixXd matl(A.rows, A.cols);
	MatrixXd matv(A.rows, A.cols);

	cv2eigen(A,mata);

	EigenSolver<MatrixXd> es(mata);
	matl = es.pseudoEigenvalueMatrix();
	matv = es.pseudoEigenvectors();

	eigen2cv(matl, L);
	eigen2cv(matv, V);
}
