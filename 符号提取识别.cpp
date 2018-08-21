/*

*/

#include "opencv2/opencv.hpp"
#include "��������.h"
using namespace std;
using namespace cv;

#define BaseIdealArea 100
/*
input: one binary image
output:base of input image
draw_flag:�Ƿ��������ͼ����Ӿ���ͼ
*/
void base_extract(Mat src, vector<Base> &base_all,bool ImshowFlag=0)
{
	vector<vector<Point>>contours;//��������
	vector<Vec4i>hierarchy;
	vector<Rect>boundrect;//������Ӿ��μ��� ��С��contours.size()
	//vector<RotatedRect>minRect;//������С�������͹������ ��С��contours.size()
	vector<Mat> imgROI; //roi���򼯺ϣ���Ԫͼ������
	//��ʼ������ͼ
	Mat contours_img = Mat::zeros(src.rows, src.cols, CV_8UC3);
	//��ʼ�� ��Ӿ��� ͼ
	Mat boundrect_img = Mat::zeros(contours_img.rows, contours_img.cols, CV_8UC3);
	//��С�������͹��
	//Mat minRect_img = Mat::zeros(contours_img.rows, contours_img.cols, CV_8UC3);

	/*ts*/
	//Mat boundrect_img_ts = Mat::zeros(contours_img.rows, contours_img.cols, CV_8UC3);
	/*ts*/

	//�ҵ����� ������ͼ�����½ǿ�ʼ����Ѱ��
	findContours(src, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//��������
	for (int index = 0; index < contours.size(); index++)//������������
  //for (int index = 0; index >=0; index=hierarchy[index][0])//������������
	{
		/*����*///ʵ�ʲ��û�������ͼ
		Scalar color(255, 255, 255);//������ɫ

		drawContours(contours_img, contours, index, color, 1, 8, hierarchy);
		/*����*/

		//boundrect.push_back(rectCenterScale(boundingRect(contours[index]), Size(6, 6)));//��Ӿ����ʵ��Ŵ�
		boundrect.push_back(boundingRect(contours[index]));
		//minRect.push_back(minAreaRect(contours[index]));
		/*����*/
		//���� ��Ӿ���
		rectangle(boundrect_img, boundrect[index], Scalar(0, 0, 255));
		//yx_drawRotatedRect(src, minRect[index], Scalar(255, 255, 255));
		//�Ŵ���Ӿ��Σ���ʾ��
		//rectangle(boundrect_img, rectCenterScale(boundrect[index], Size(4,4)), Scalar(255, 255, 255));
		//rectangle(binary_img, rectCenterScale(boundrect[index],Size(4, 4)), Scalar(255, 255, 255));
		/*����*/

		//��������Ӿ����趨Ϊroi����ȡ��ÿһ����Ԫ��
		//boundrect �ṩroi����
		//roi����Ϊ��ֵ��ͼbinary_img;
		Mat roi = src(boundrect[index]);
		//�洢��ֵ��ͼ�����ҵ������л�Ԫ �����ܱ�֤�ҵ����л�Ԫ����������ʶ��

		//����base������
		Base base1;//���ܻ�Ԫ
		base1.rect_coordinate = boundrect[index].tl();
		base1.img = roi;
		base1.boundrect = boundrect[index];
		base1.contour = contours[index];
		base1.rect_center_coor = getCenterPoint(boundrect[index]);
		base1.area = boundrect[index].area();

		//if (FilterByArea(base1))
		//{
			base_all.push_back(base1);//���л�Ԫ
		//}
		

	  //��ͼ��
		if (ImshowFlag == 1)
		{
			imshow("����ͼ", contours_img);
			imshow("��Ӿ���ͼ", boundrect_img);
			//imshow("��С����", minRect_img);
		}
	}



}

/*
����Ȳ��ܳ���2��������ɸѡ��Ч��Ԫ
ʵ����������������ƽ�棬��С����ƽ�淵��ͼ���л�Ԫ���������Ϊ���ɸѡ��ֵ
Ŀ���Ԫ���������ֵҪ�󷵻� 1
���򷵻� 0
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

//ʶ����ţ����ػ�Ԫ���ű�ʾ�ġ��������ꡱ��pair��firstΪ�������꣬pair��secondΪ��������
pair<int,int> SymbolRecognition(Base base)
{
	return pair<int, int>(0, 0);
}