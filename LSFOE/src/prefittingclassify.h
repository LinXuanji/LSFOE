#pragma once
#ifndef _PREFITTINGCLASSIFY_H
#define _PREFITTINGCLASSIFY_H

#include <opencv.hpp>
#include <vector>

/*@bref:ͼ��ѧ�ղ���
@param:[img] [����Ķ�ֵ��ͼ��]
@param:[elem] [�ղ���ʹ�õĺ���״:
MORPH_RECT ���κ�
MORPH_CROSS ʮ�ֺ�
MORPH_ELLIPSE ��Բ��]
@param:[size] [�ղ���ʹ�õĺ˴�С]
@return:[Mat] [�ղ������ͼ��]*/
cv::Mat performClosing(cv::Mat img, int elem, int size);
/*@bref:��Ŀ����Ϊ��ɫ
@param:[img] [�����ԭʼͼ��]
@return:[Mat] [Ŀ������]*/
cv::Mat BGR(cv::Mat img);
/*@bref:Ѱ�һ����е�Ŀ�겢����Ŀ������
@param:[binimg] [����Ķ�ֵ��ͼ��]
@return:[bool] [Ŀ���Ƿ���������]*/
bool ROIlocate(cv::Mat oriimg);
/*@bref:װ�ؾ���ƥ���㷨�е�ƥ���
@return:[vector<Mat>] [ƥ�������]*/
std::vector<cv::Mat> loadMask(void);
/*@bref:Ѱ�ҵ�ǰƥ������չ�����
@param:[point] [��ǰƥ�������]
@param:[img] [����Ķ�ֵ��ͼ��]
@return:[Mat] [��ǰƥ������չ�����]*/
cv::Mat findPointMat(cv::Point2f point, cv::Mat img);
/*@bref:����ƥ����Ѱ����һĿ���
@param:[i] [ƥ����������еı�ǩ]
@param:[lastpoint] [��ǰĿ���]
@return:[Point2f] [��һĿ�������]*/
cv::Point2f findNextPoint(int i, cv::Point2f lastpoint);
/*@bref:�ж��������Ƿ����
@param:[a] [��Ҫ�жϵľ���]
@param:[b] [��Ҫ�жϵľ���]
@return:[bool] [true:���������
false:�����󲻵�]*/
bool isEqual(cv::Mat a, cv::Mat b);
/*@bref:�ɼ�Ŀ��߽�����
@param:[img] [����Ķ�ֵ��ͼ��]
@param:[collect] [���صı߽�㼯ͼ��]
@param:[mask] [ƥ�������]
@return:[vector<Point2f>] [�߽����ߵ㼯]*/
std::vector<cv::Point2f> collectBoundary(cv::Mat img, cv::Mat& collect, std::vector<cv::Mat> mask);
std::vector<cv::Point2f> collectBoundary(cv::Mat img, std::vector<cv::Mat> mask);

#endif // !_PREFITTINGCLASSIFY_H

