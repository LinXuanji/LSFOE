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
	double xc;   // ��������x
	double yc;   // ��������y
	double ma;   // ����
	double sma;	 // ����
	double theta;// ���
}ellippara;
/*
*@bref: ��Բ����࣬�ṩ����С���˷���Բ��ϵķ����ӿڡ�
*@author: ColdDewStudio
*@version: 0.0.1
*/
class Ellipses
{
private:
	cv::Mat H;								// ��������
	cv::Mat X;								// δ֪������,X = [x^2,x*y,y^2,x,y,1]T
	cv::Mat W;								// ���̲�������
	cv::Mat S;								// XXT
	double A, B, C, D, E, F;				// ���̲���
	std::vector<cv::Point2f> bp;// �߽�㼯
	ellippara ellipseparameters;			// ��Բ���β�����
public:
	Ellipses();
	~Ellipses();

	/*@bref:��ȡ�߽�㼯
	ͨ���ú������ɼ�����Ŀ��߽�㼯���뵽���д���
	@param:[boundarypoints] [�߽�㼯]*/
	void GetBoundaryPoints(std::vector<cv::Point2f> boundarypoints);
	/*@bref:����δ֪������
	���õ���ı߽�㼯����һ��6�б߽�㼯�����е�δ
	֪������X��X = [x^2,x*y,y^2,x,y,1]T*/
	void BuildUnknownNumberMat();
	/*@bref:����Բ���β���
	������Բ���̲����������Բ*/
	void SolveEllipses();
	/*@bref:������Բ���β���
	return:[ellippara]*/
	ellippara FittingSolution();
};

/*@bref:����������ʸ������V��ʹ�����MATLAB��׼
@param:[V] [�����򻯵�����ʸ������]
@return:[Mat]
@deprecated*/
cv::Mat EigenVSort(cv::Mat V);
/*@bref:�����������ֵ����������
@param:[A] [������Ҫ���ľ���]
@param:[L] [���������ֵ�ԽǾ���]
@param:[V] [�����������������]*/
void eig(cv::Mat A, cv::Mat& L, cv::Mat& V);

#endif // !_ELLIPSESFITTING


