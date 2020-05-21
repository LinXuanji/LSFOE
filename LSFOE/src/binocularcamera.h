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
*@bref:˫Ŀ������࣬�ṩ������������궨������ȹ��ܽӿ�
*@author:ColdDewStudio
*@version: 0.0.1
*/
class Camera
{
private:
	cv::VideoCapture cap;					//�����
	cv::Mat frame;							//��ȡ�ļ�¼֡
	cv::Mat doubleimg;						//˫Ŀͼ��
	cv::Mat leftview;						//����ͼ
	cv::Mat rightview;						//����ͼ
	cv::Mat leftcameramatrix;				//������ڲξ���
	cv::Mat leftdistortion;					//�������������
	cv::Mat rightcameramatrix;				//������ڲξ���
	cv::Mat rightdistortion;				//�������������
	int numberofshots;						//��������
	bool zoom;								//ͼ������

	cv::Size imgsize;							//ͼ���С

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

	/*@bref:��ָ������ͷ
	@param:[cameranum] [Ҫ�򿪵�����ͷ���]*/
	void Open(int cameranum);
	/*@bref:��ȡ����ͼ
	@param:[mode] [�Ƿ���ʾ��ȡ��ͼ��:
	CAMERA_SHOWN ��ʾ
	CAMERA_HIDDEN ����ʾ]*/
	cv::Mat GetLeftimg(int mode);
	/*@bref:��ȡ����ͼ
	@param:[mode] [�Ƿ���ʾ��ȡ��ͼ��:
	CAMERA_SHOWN ��ʾ
	CAMERA_HIDDEN ����ʾ]*/
	cv::Mat GetRightimg(int mode);
	/*@bref:���գ�֧������ͷѡ��֧������
	@param:[camera] [����ʹ�õ�����ͷ
	CAMERA_LEFT ������ͷ
	CAMERA_RIGHT ������ͷ
	CAMERA_BOTH ˫����ͷ]
	@param:[num] [��������]*/
	void Snapshot(int camera, int num);
	/*@bref:����ͼ��
	@param:[mode] [��С��Ŵ�:
	ZOOM_PLUS:�Ŵ�
	ZOOM_OUT:��С]*/
	void Zoom(int mode);
	/*@bref:װ�����������*/
	void loadcameraparams(void);
	/*@bref:����У��*/
	void Stereocorrection(void);
	/*@bref:��ά���*/
	void SGBM(void);
	/*@bref:��ȡĿ���3ά����*/
	void Get3Dpos(int x, int y);
};

#endif // !_BINOCULARCAMERA_H

