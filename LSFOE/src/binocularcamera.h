#pragma once
#ifndef _BINOCULARCAMERA_H
#define _BINOCULARCAMERA_H

#define CAMERA_SHOWN	0x00000001
#define CAMERA_HIDDEN	0x00000002
#define CAMERA_LEFT		0x00000003
#define CAMERA_RIGHT	0x00000004
#define CAMERA_BOTH		0x00000005
#define ZOOM_PLUS		0x00000006
#define ZOOM_OUT		0x00000007

#include <opencv.hpp>
#include <iostream>

/*
*@bref:双目摄像机类，提供了相机驱动，标定，与测距等功能接口
*@author:ColdDewStudio
*@version: 0.0.1
*/
class Camera
{
private:
	cv::VideoCapture cap;					//摄像机
	cv::Mat frame;							//获取的记录帧
	cv::Mat doubleimg;						//双目图像
	cv::Mat leftview;						//左视图
	cv::Mat rightview;						//右视图
	cv::Mat leftcameramatrix;				//左相机内参矩阵
	cv::Mat leftdistortion;					//左相机畸变向量
	cv::Mat rightcameramatrix;				//右相机内参矩阵
	cv::Mat rightdistortion;				//右相机畸变向量
	int numberofshots;						//连拍数量
	bool zoom;								//图像缩放

	cv::Size imgsize;							//图像大小

	cv::Mat mapl1, mapl2, mapr1, mapr2;
	cv::Mat R1, R2, P1, P2;
	cv::Mat Q;
	cv::Mat R, T;

	cv::Mat xyz;

	cv::Rect validROIL;
	cv::Rect validROIR;

	cv::Ptr<cv::StereoBM> bm;
	int blockSize = 8, uniquenessRatio = 5, numDisparities = 16;
	
public:
	Camera();
	Camera(int cameranum);
	~Camera();

	/*@bref:打开指定摄像头
	@param:[cameranum] [要打开的摄像头编号]*/
	void Open(int cameranum);
	/*@bref:获取左视图
	@param:[mode] [是否显示获取的图像:
	CAMERA_SHOWN 显示
	CAMERA_HIDDEN 不显示]*/
	cv::Mat GetLeftimg(int mode);
	/*@bref:获取右视图
	@param:[mode] [是否显示获取的图像:
	CAMERA_SHOWN 显示
	CAMERA_HIDDEN 不显示]*/
	cv::Mat GetRightimg(int mode);
	/*@bref:拍照，支持摄像头选择，支持连拍
	@param:[camera] [拍照使用的摄像头
	CAMERA_LEFT 左摄像头
	CAMERA_RIGHT 右摄像头
	CAMERA_BOTH 双摄像头]
	@param:[num] [拍照数量]*/
	void Snapshot(int camera, int num);
	/*@bref:缩放图像
	@param:[mode] [缩小或放大:
	ZOOM_PLUS:放大
	ZOOM_OUT:缩小]*/
	void Zoom(int mode);
	/*@bref:装载摄像机参数*/
	void loadcameraparams(void);
	/*@bref:立体校正*/
	void Stereocorrection(void);
	/*@bref:三维测距*/
	void SGBM(void);
	/*@bref:获取目标点3维坐标*/
	void Get3Dpos(int x, int y);
};

#endif // !_BINOCULARCAMERA_H

