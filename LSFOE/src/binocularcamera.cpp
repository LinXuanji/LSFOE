#include "binocularcamera.h"

using namespace cv;
using namespace std;

Camera::Camera()
{
	loadcameraparams();
	numberofshots = -1;
	zoom = false;
}

Camera::Camera(int cameranum)
{
	loadcameraparams();
	cap.open(cameranum);
	numberofshots = -1;
	zoom = false;
}

Camera::~Camera()
{
	cap.release();
}

void Camera::Open(int cameranum)
{
	loadcameraparams();
	cap.open(cameranum);
	numberofshots = -1;
	zoom = false;
}

cv::Mat Camera::GetLeftimg(int mode)
{
	cap >> frame;
	resize(frame, doubleimg, Size(1280, 480), (0, 0), (0, 0), INTER_AREA);
	leftview = doubleimg(Rect(0, 0, 640, 480));

	if (zoom)
	{
		leftview = leftview(Rect(160, 120, 320, 240));
		resize(leftview, leftview, Size(640, 480), (0, 0), (0, 0), INTER_AREA);
	}

	imgsize = leftview.size();
	
	if (mode == CAMERA_SHOWN) imshow("leftview", leftview);

	return leftview;
}

cv::Mat Camera::GetRightimg(int mode)
{
	cap >> frame;
	resize(frame, doubleimg, Size(1280, 480), (0, 0), (0, 0), INTER_AREA);
	rightview = doubleimg(Rect(640, 0, 640, 480));

	if (zoom)
	{
		rightview = rightview(Rect(160, 120, 320, 240));
		resize(rightview, rightview, Size(640, 480), (0, 0), (0, 0), INTER_AREA);
	}

	if (mode == CAMERA_SHOWN) imshow("rightview", rightview);

	return rightview;
}

void Camera::Snapshot(int camera, int num)
{
	std::ostringstream streaml;
	std::ostringstream streamr;
	if (numberofshots == -1)
	{
		numberofshots = num;
		system("mkdir output");
	}

	if (numberofshots != 0)
	{
		switch (camera)
		{
		case CAMERA_LEFT: {
			streaml << "./output/" << num - numberofshots << "left.jpg";
			imwrite(streaml.str(), leftview);
		}break;
		case CAMERA_RIGHT: {
			streamr << "./output/" << num - numberofshots << "right.jpg";
			imwrite(streamr.str(), rightview);
		}break;
		case CAMERA_BOTH: {
			streaml << "./output/" << num - numberofshots << "left.jpg";
			imwrite(streaml.str(), leftview);
			streamr << "./output/" << num - numberofshots << "right.jpg";
			imwrite(streamr.str(), rightview);
		}break;
		}

		numberofshots--;
	}
}

void Camera::Zoom(int mode)
{
	switch (mode)
	{
	case ZOOM_PLUS: zoom = true; break;
	case ZOOM_OUT: zoom = false; break;
	}
}

void Camera::loadcameraparams(void)
{
	T = Mat::zeros(3, 1, CV_64F);
	T.at<double>(0, 0) = -60.4269776937938;
	T.at<double>(1, 0) = -0.201073421941211;
	T.at<double>(2, 0) = 0.309234661010653;

	R = Mat::eye(3, 3, CV_64F);
	R.at<double>(0, 0) = 0.999988657964642;
	R.at<double>(0, 1) = -0.00244507299231352;
	R.at<double>(0, 2) = 0.00408724358655073;
	R.at<double>(1, 0) = 0.00242438080268041;
	R.at<double>(1, 1) = 0.999984259576937;
	R.at<double>(1, 2) = 0.00505993834834636;
	R.at<double>(2, 0) = -0.00409955117020580;
	R.at<double>(2, 1) = -0.00504997192345962;
	R.at<double>(2, 2) = 0.999978845508131;

	leftcameramatrix = Mat::eye(3, 3, CV_64F);
	leftcameramatrix.at<double>(0, 0) = 425.025304127266;
	leftcameramatrix.at<double>(0, 1) = 0.00731868726689901;
	leftcameramatrix.at<double>(0, 2) = 349.235797889362;
	leftcameramatrix.at<double>(1, 1) = 424.140745691164;
	leftcameramatrix.at<double>(1, 2) = 256.660401733645;

	rightcameramatrix = Mat::eye(3, 3, CV_64F);
	rightcameramatrix.at<double>(0, 0) = 425.674050862817;
	rightcameramatrix.at<double>(0, 1) = 0.191181937542449;
	rightcameramatrix.at<double>(0, 2) = 349.535155589642;
	rightcameramatrix.at<double>(1, 1) = 425.021946619558;
	rightcameramatrix.at<double>(1, 2) = 254.118049826843;

	leftdistortion = Mat::zeros(5, 1, CV_64F);
	leftdistortion.at<double>(0, 0) = 0.0173240561104062;
	leftdistortion.at<double>(1, 0) = -0.0274112722325813;
	leftdistortion.at<double>(2, 0) = -0.000560021992039056;
	leftdistortion.at<double>(3, 0) = 0.00123289787558843;
	leftdistortion.at<double>(4, 0) = 0.0788731549018870;

	rightdistortion = Mat::zeros(5, 1, CV_64F);
	rightdistortion.at<double>(0, 0) = 0.0162602478150832;
	rightdistortion.at<double>(1, 0) = -0.0140110824528280;
	rightdistortion.at<double>(2, 0) = -3.96815938329112e-05;
	rightdistortion.at<double>(3, 0) = -0.00112135850586720;
	rightdistortion.at<double>(4, 0) = -0.00823583793419864;
}

void Camera::Stereocorrection(void)
{
	stereoRectify(leftcameramatrix, leftdistortion, rightcameramatrix, rightdistortion, imgsize,
		R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, -1, 
		imgsize, &validROIL, &validROIR);

	initUndistortRectifyMap(leftcameramatrix, leftdistortion, R1,P1, imgsize, CV_16SC2, mapl1, mapl2);
	remap(leftview, leftview, mapl1, mapl2, INTER_LINEAR);

	initUndistortRectifyMap(rightcameramatrix, rightdistortion, R2, P2, imgsize, CV_16SC2, mapr1, mapr2);
	remap(rightview, rightview, mapr1, mapr2, INTER_LINEAR);
}

void Camera::SGBM(void)
{
	Ptr<StereoBM> bm = StereoBM::create(16, 9);

	bm->setBlockSize(2 * blockSize + 5);     //SAD窗口大小，5~21之间为宜
	
	bm->setROI1(validROIL);
	bm->setROI2(validROIR);
	bm->setPreFilterCap(31);
	bm->setMinDisparity(0);  //最小视差，默认值为0, 可以是负值，int型
	bm->setNumDisparities(numDisparities * 16 + 16);//视差窗口，即最大视差值与最小视差值之差,窗口大小必须是16的整数倍，int型
	bm->setTextureThreshold(10);
	bm->setUniquenessRatio(uniquenessRatio);//uniquenessRatio主要可以防止误匹配
	bm->setSpeckleWindowSize(100);
	bm->setSpeckleRange(32);
	bm->setDisp12MaxDiff(-1);
	Mat disp, disp8, color, grayl, grayr;
	color = Mat::ones(Size(imgsize), CV_8UC3);
	cvtColor(leftview, grayl, COLOR_BGR2GRAY);
	cvtColor(rightview, grayr, COLOR_BGR2GRAY);
	bm->compute(grayl, grayr, disp);//输入图像必须为灰度图
	disp.convertTo(disp8, CV_8U, 255 / ((numDisparities * 16 + 16) * 16.));//计算出的视差是CV_16S格式
	reprojectImageTo3D(disp, xyz, Q, true); //在实际求距离时，ReprojectTo3D出来的X / W, Y / W, Z / W都要乘以16(也就是W除以16)，才能得到正确的三维坐标信息。
	xyz = xyz * 16;

	float max_val = 255.0f;
	float map[8][4] = { { 0,0,0,114 },{ 0,0,1,185 },{ 1,0,0,114 },{ 1,0,1,174 },
	{ 0,1,0,114 },{ 0,1,1,185 },{ 1,1,0,114 },{ 1,1,1,0 } };
	float sum = 0;
	for (int i = 0; i < 8; i++)
		sum += map[i][3];

	float weights[8]; // relative   weights  
	float cumsum[8];  // cumulative weights  
	cumsum[0] = 0;
	for (int i = 0; i < 7; i++) {
		weights[i] = sum / map[i][3];
		cumsum[i + 1] = cumsum[i] + map[i][3] / sum;
	}

	// for all pixels do  
	for (int v = 0; v < imgsize.height; v++) {
		for (int u = 0; u < imgsize.width; u++) {

			// get normalized value  
			float val = std::min(std::max(disp8.data[v * imgsize.width + u] / max_val, 0.0f), 1.0f);

			// find bin  
			int i;
			for (i = 0; i < 7; i++)
				if (val < cumsum[i + 1])
					break;

			// compute red/green/blue values  
			float   w = 1.0 - (val - cumsum[i]) * weights[i];
			uchar r = (uchar)((w * map[i][0] + (1.0 - w) * map[i + 1][0]) * 255.0);
			uchar g = (uchar)((w * map[i][1] + (1.0 - w) * map[i + 1][1]) * 255.0);
			uchar b = (uchar)((w * map[i][2] + (1.0 - w) * map[i + 1][2]) * 255.0);
			//rgb内存连续存放  
			color.data[v * imgsize.width * 3 + 3 * u + 0] = b;
			color.data[v * imgsize.width * 3 + 3 * u + 1] = g;
			color.data[v * imgsize.width * 3 + 3 * u + 2] = r;
		}
	}

	imshow("disparity", color);
}

int Camera::Get3Dpos(float x, float y, float alpha, angle& ang)
{
	Mat img = Mat::zeros(800,800,CV_8UC3);

	/*int xback, yback;
	cout << xyz.at<Vec3f>(x, y) << endl;*/

	double L, X, Y, ALP;
	L = xyz.at<Vec3f>(x, y)[2] + 15;

	ALP = 90 + (1500 - alpha) * 0.135;
	X = L * sin(ALP / 180 * PI) + MLEN * cos(ALP / 180 * PI);
	Y = NLEN + MLEN * sin(ALP / 180 * PI) - L * cos(ALP / 180 * PI);

	cout << "L:" << L << endl;
	cout << "ALP:" << ALP << endl;

	cout << "X:" << X << endl;
	cout << "Y:" << Y << endl;

	double theta1, theta2, theta3, xc, yc, k1, k2, c2, s2;

	xc = X - l2 - 20;
	yc = Y - 45;

	circle(img, Point(200 + xc + l2, 400 - yc), 5, Scalar(0, 0, 255), -1, 8);
	circle(img, Point(200, 400), 5, Scalar(255, 255, 255), -1, 8);
	//yc = Y;

	c2 = (xc * xc + yc * yc - l0 * l0 - l1 * l1) / (2 * l0 * l1);

	if (c2 < -1 || c2>1)
	{
		return 1;
	}

	s2 = sqrt(1 - c2 * c2);
	theta2 = atan2(s2, c2);

	k1 = l0 + l1 * c2;
	k2 = l1 * s2;

	theta1 = atan2(yc, xc) - atan2(k2, k1);
	if (theta1<0 || theta1>PI)
	{
		s2 = -sqrt(1 - c2 * c2);
		theta2 = atan2(s2, c2);

		k1 = l0 + l1 * c2;
		k2 = l1 * s2;

		theta1 = atan2(yc, xc) - atan2(k2, k1);
	}

	theta3 = -theta1 - theta2;

	double xds;

	xds = l0 * cos(theta1) + l1 * cos(theta1 + theta2) + l2 * cos(theta1 + theta2 + theta3);

	line(img, Point(200, 400),Point(200 + l0 * cos(theta1), 400 - l0 * sin(theta1)), Scalar(255, 0, 255), 2, 8);
	line(img, Point(200 + l0 * cos(theta1), 400 - l0 * sin(theta1)),
		Point(200 + l1 * cos(theta1 + theta2) + l0 * cos(theta1), 400 - (l1 * sin(theta1 + theta2) + l0 * sin(theta1))),
		Scalar(255, 0, 0), 2, 8);
	line(img, Point(200 + l1 * cos(theta1 + theta2) + l0 * cos(theta1), 400 - (l1 * sin(theta1 + theta2) + l0 * sin(theta1))),
		Point(200 + l2 * cos(theta1 + theta2 + theta3) + l1 * cos(theta1 + theta2) + l0 * cos(theta1), 
			400 - (l2 * sin(theta1 + theta2 + theta3) + l1 * sin(theta1 + theta2) + l0 * sin(theta1))),
		Scalar(0, 255, 0), 2, 8);

	imshow("pic", img);

	theta1 = theta1 * 180 / PI;
	theta2 = theta2 * 180 / PI;
	theta3 = theta3 * 180 / PI;

	cout << "t1:" << theta1 << endl;
	cout << "t2:" << theta2 << endl;
	cout << "t3:" << theta3 << endl;

	//theta2 = theta1 + theta2;
	//theta3 = theta1 + theta2 + theta3;

	cout << "xds:" << xds << endl;

	//ang.th1 = (2000 * (90.0 - theta1) / 180.0 + 500.0);
	//ang.th2 = (2000 * (90.0 - theta2) / 180.0 + 500.0);
	//ang.th3 = (2000 * (theta3) / 180.0 + 500.0);

	ang.th1 = 1500 - (90 - theta1) / 0.135;
	if (ang.th1 > 2500)ang.th1 = 2500;
	if (ang.th1 < 500)ang.th1 = 500;
	ang.th2 = 1500 - theta2 / 0.135;
	if (ang.th2 > 2500)ang.th2 = 2500;
	if (ang.th2 < 500)ang.th2 = 500;
	ang.th3 = 1500 - theta3 / 0.135;
	if (ang.th3 > 2500)ang.th3 = 2500;
	if (ang.th3 < 500)ang.th3 = 500;

	cout << ang.th1 << endl;
	cout << ang.th2 << endl;
	cout << ang.th3 << endl;

	return 0;
}