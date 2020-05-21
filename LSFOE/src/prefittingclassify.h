#pragma once
#ifndef _PREFITTINGCLASSIFY_H
#define _PREFITTINGCLASSIFY_H

#include <opencv.hpp>
#include <vector>

/*@bref:图形学闭操作
@param:[img] [输入的二值化图像]
@param:[elem] [闭操作使用的核形状:
MORPH_RECT 矩形核
MORPH_CROSS 十字核
MORPH_ELLIPSE 椭圆核]
@param:[size] [闭操作使用的核大小]
@return:[Mat] [闭操作后的图像]*/
cv::Mat performClosing(cv::Mat img, int elem, int size);
/*@bref:将目标标记为白色
@param:[img] [输入的原始图像]
@return:[Mat] [目标区域]*/
cv::Mat BGR(cv::Mat img);
/*@bref:寻找环境中的目标并返回目标区域
@param:[binimg] [输入的二值化图像]
@return:[bool] [目标是否在区域内]*/
bool ROIlocate(cv::Mat oriimg);
/*@bref:装载矩阵匹配算法中的匹配核
@return:[vector<Mat>] [匹配核向量]*/
std::vector<cv::Mat> loadMask(void);
/*@bref:寻找当前匹配点的扩展矩阵核
@param:[point] [当前匹配点坐标]
@param:[img] [输入的二值化图像]
@return:[Mat] [当前匹配点的扩展矩阵核]*/
cv::Mat findPointMat(cv::Point2f point, cv::Mat img);
/*@bref:根据匹配结果寻找下一目标点
@param:[i] [匹配核在向量中的标签]
@param:[lastpoint] [当前目标点]
@return:[Point2f] [下一目标点坐标]*/
cv::Point2f findNextPoint(int i, cv::Point2f lastpoint);
/*@bref:判断两矩阵是否相等
@param:[a] [需要判断的矩阵]
@param:[b] [需要判断的矩阵]
@return:[bool] [true:两矩阵相等
false:两矩阵不等]*/
bool isEqual(cv::Mat a, cv::Mat b);
/*@bref:采集目标边界曲线
@param:[img] [输入的二值化图像]
@param:[collect] [返回的边界点集图像]
@param:[mask] [匹配核向量]
@return:[vector<Point2f>] [边界曲线点集]*/
std::vector<cv::Point2f> collectBoundary(cv::Mat img, cv::Mat& collect, std::vector<cv::Mat> mask);
std::vector<cv::Point2f> collectBoundary(cv::Mat img, std::vector<cv::Mat> mask);

#endif // !_PREFITTINGCLASSIFY_H

