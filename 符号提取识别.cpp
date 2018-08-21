/*

*/

#include "opencv2/opencv.hpp"
#include "符号阵列.h"
using namespace std;
using namespace cv;

#define BaseIdealArea 100
/*
input: one binary image
output:base of input image
draw_flag:是否绘制轮廓图和外接矩形图
*/
void base_extract(Mat src, vector<Base> &base_all,bool ImshowFlag=0)
{
	vector<vector<Point>>contours;//轮廓集合
	vector<Vec4i>hierarchy;
	vector<Rect>boundrect;//定义外接矩形集合 大小：contours.size()
	//vector<RotatedRect>minRect;//定义最小面积矩形凸包集合 大小：contours.size()
	vector<Mat> imgROI; //roi区域集合，基元图案集合
	//初始化轮廓图
	Mat contours_img = Mat::zeros(src.rows, src.cols, CV_8UC3);
	//初始化 外接矩形 图
	Mat boundrect_img = Mat::zeros(contours_img.rows, contours_img.cols, CV_8UC3);
	//最小面积矩形凸包
	//Mat minRect_img = Mat::zeros(contours_img.rows, contours_img.cols, CV_8UC3);

	/*ts*/
	//Mat boundrect_img_ts = Mat::zeros(contours_img.rows, contours_img.cols, CV_8UC3);
	/*ts*/

	//找到轮廓 从整幅图像右下角开始按行寻找
	findContours(src, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//绘制轮廓
	for (int index = 0; index < contours.size(); index++)//遍历所有轮廓
  //for (int index = 0; index >=0; index=hierarchy[index][0])//遍历所有轮廓
	{
		/*调试*///实际不用绘制轮廓图
		Scalar color(255, 255, 255);//轮廓颜色

		drawContours(contours_img, contours, index, color, 1, 8, hierarchy);
		/*调试*/

		//boundrect.push_back(rectCenterScale(boundingRect(contours[index]), Size(6, 6)));//外接矩形适当放大
		boundrect.push_back(boundingRect(contours[index]));
		//minRect.push_back(minAreaRect(contours[index]));
		/*调试*/
		//绘制 外接矩形
		rectangle(boundrect_img, boundrect[index], Scalar(0, 0, 255));
		//yx_drawRotatedRect(src, minRect[index], Scalar(255, 255, 255));
		//放大外接矩形，演示用
		//rectangle(boundrect_img, rectCenterScale(boundrect[index], Size(4,4)), Scalar(255, 255, 255));
		//rectangle(binary_img, rectCenterScale(boundrect[index],Size(4, 4)), Scalar(255, 255, 255));
		/*调试*/

		//将轮廓外接矩形设定为roi，提取出每一个基元；
		//boundrect 提供roi参数
		//roi本体为二值化图binary_img;
		Mat roi = src(boundrect[index]);
		//存储二值化图像上找到的所有基元 并不能保证找到所有基元，可能有误识别

		//生成base类数据
		Base base1;//可能基元
		base1.rect_coordinate = boundrect[index].tl();
		base1.img = roi;
		base1.boundrect = boundrect[index];
		base1.contour = contours[index];
		base1.rect_center_coor = getCenterPoint(boundrect[index]);
		base1.area = boundrect[index].area();

		//if (FilterByArea(base1))
		//{
			base_all.push_back(base1);//所有基元
		//}
		

	  //绘图区
		if (ImshowFlag == 1)
		{
			imshow("轮廓图", contours_img);
			imshow("外接矩形图", boundrect_img);
			//imshow("最小矩形", minRect_img);
		}
	}



}

/*
长宽比不能超过2倍，初步筛选有效基元
实验测得量程内最大距离平面，最小距离平面返回图像中基元的面积，作为面积筛选阈值
目标基元面积符合阈值要求返回 1
否则返回 0
*/
bool FilterByArea(Base base)
{
	int minArea = 0;
	int maxArea = 0;
	if (base.area>=minArea&&base.area<=maxArea)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}

//识别符号，返回基元符号表示的“行列坐标”，pair。first为行向坐标，pair。second为列向坐标
pair<int,int> SymbolRecognition(Base base)
{
	return pair<int, int>(0, 0);
}