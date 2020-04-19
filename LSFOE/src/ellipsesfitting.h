#pragma once

#ifndef _ELLIPSESFITTING
#define _ELLIPSESFITTING

#include <Dense>
#include <Core>
#include <opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include <vector>

#define PI 3.1415926


typedef struct ellip // Ellipse parameters
{
	double xc;   // 几何中心x
	double yc;   // 几何中心y
	double ma;   // 长轴
	double sma;	 // 短轴
	double theta;// 倾角
}ellippara;
/*
*@bref: 椭圆拟合类，提供了最小二乘法椭圆拟合的方法接口。
*@author: ColdDewStudio
*@version: 0.0.1
*/
class Ellipses
{
private:
	cv::Mat H;								// 辅助矩阵
	cv::Mat X;								// 未知数矩阵,X = [x^2,x*y,y^2,x,y,1]T
	cv::Mat W;								// 方程参数矩阵
	cv::Mat S;								// XXT
	double A, B, C, D, E, F;				// 方程参数
	std::vector<cv::Point2f> bp;// 边界点集
	ellippara ellipseparameters;			// 椭圆几何参数集
public:
	Ellipses();
	~Ellipses();

	/*@bref:获取边界点集
	通过该函数将采集到的目标边界点集导入到类中处理
	@param:[boundarypoints] [边界点集]*/
	void GetBoundaryPoints(std::vector<cv::Point2f> boundarypoints);
	/*@bref:建立未知数矩阵
	利用导入的边界点集建立一个6行边界点集个数列的未
	知数矩阵X，X = [x^2,x*y,y^2,x,y,1]T*/
	void BuildUnknownNumberMat();
	/*@bref:解椭圆几何参数
	计算椭圆方程参数，拟合椭圆*/
	void SolveEllipses();
	/*@bref:返回椭圆几何参数
	return:[ellippara]*/
	ellippara FittingSolution();
};

/*@bref:重排列特征矢量矩阵V，使其符合MATLAB标准
@param:[V] [待规则化的特征矢量矩阵]
@return:[Mat]
@deprecated*/
cv::Mat EigenVSort(cv::Mat V);
/*@bref:求解矩阵的特征值和特征向量
@param:[A] [输入需要求解的矩阵]
@param:[L] [输出的特征值对角矩阵]
@param:[V] [输出的特征向量矩阵]*/
void eig(cv::Mat A, cv::Mat& L, cv::Mat& V);

#endif // !_ELLIPSESFITTING


