/*

一些辅助函数


*/
#include "opencv2/opencv.hpp"
#include "符号阵列.h"

//坐标变换函数
#define cam_X_angle (15/180*3.1415)	//相机与世界坐标系X轴夹锐角 单位
#define cam_f	//相机焦距


using namespace std;
using namespace cv;
//围绕矩形中心缩放  
//size:长宽增加的像素值
Rect rectCenterScale(Rect rect, Size size)
{
	rect = rect + size;
	Point pt;
	pt.x = cvRound(size.width / 2.0);
	pt.y = cvRound(size.height / 2.0);
	return (rect - pt);
}

//获取矩形中心点
Point getCenterPoint(Rect rect)
{
	Point cpt;
	cpt.x = rect.x + cvRound(rect.width / 2.0);
	cpt.y = rect.y + cvRound(rect.height / 2.0);
	return cpt;
}

/*
绘制旋转矩形
imshow_flag 是否显示图像
rrec RotatedRect类
*/
void yx_drawRotatedRect(Mat &img,RotatedRect rrec,Scalar &color,int thickness, bool imshow_flag)
{
	
	vector<Point2i>pts;	//存储矩形四个顶点
	Point2f vertices[4];      //定义矩形的4个顶点
	rrec.points(vertices);   //计算矩形的4个顶点
	for (size_t i = 0; i < 4; i++)
	{
		pts.push_back(vertices[i]);
	}
	//?? pts被要求为Point2i类型？？
	polylines(img, pts, 1, color, thickness, 8, 0);
	if (imshow_flag)
	{
		imshow("yx_RotatedRect", img);
	}
}

/*
	找出整数序列中出现次数最多的元素
	如果众数有多个，则随机选取一个作为结果
*/
int findMode(vector<int> array)
{

}

//坐标变换 相机-->世界
/*
func:相机左上角坐标系->相机中心坐标系->世界坐标系

输入:相机平面二维点坐标(左上角坐标系)
全局变量/宏定义:相机与世界坐标系X轴夹锐角
相机中心点

输出:输入点在世界坐标系下坐标X与Z比值,计算深度用


*/
double ct_cam2world(Point p_in)
{
	Point2i cam_plane_center;	//相机中心点坐标(左上角坐标系下,单位:像素)
								/*cam_plane_center.x = 273;
								cam_plane_center.y = 209;*/
								//cam_plane_center.x = 920.515;
								//cam_plane_center.y = 515.660;
	cam_plane_center.x = 0;
	cam_plane_center.y = 0;
	//焦距
	/*float fx = 750.8;
	float fy = 741;*/
	/*ts*/
	double fx = 1665.26;
	double fy = 1647.54;
	//float fx = 1700;

	Point3i p_out;	//世界坐标系下坐标

					//左上角->中心
	Point p_center_coor = p_in - cam_plane_center;	//中心坐标系下输入点坐标
													//像素->物理尺寸

	double X_Z = (0 - sin(cam_X_angle) + p_center_coor.x / fx * cos(cam_X_angle)) / (0 - cos(cam_X_angle) - p_center_coor.x / fx * sin(cam_X_angle));

	return X_Z;

}

//  Method:    convertTo3Channels
//  Description: 将单通道图像转为三通道图像
//  Returns:   cv::Mat 
//  Parameter: binImg 单通道图像对象
//
Mat ChannelConvert1To3(const Mat& binImg)
{
	Mat three_channel = Mat::zeros(binImg.rows, binImg.cols, CV_8UC3);
	vector<Mat> channels;
	for (int i = 0; i<3; i++)
	{
		channels.push_back(binImg);
	}
	merge(channels, three_channel);
	return three_channel;
}