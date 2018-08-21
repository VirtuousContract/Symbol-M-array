/*
	Ԥ����cpp
	1.��̬��ֵ��,��̬��ֵ��������ڶ�
	2.�˲�����ѡ��
	3.ȥ��ͼ��ڶ�

	���Σ���ֵ��blocksize��C

*/
#include "opencv2/opencv.hpp"
#include "��������.h"
using namespace std;
using namespace cv;

/*
input_img:�������ͼ������/·��
dst����ֵ��ͼ��
*/
void preprocessing(String input_img, Mat& dstImg)
{
	Mat srcImg; //ԭͼ
	Mat binary_img;//��ֵ��ͼ
	//Mat b2;
	srcImg = imread(input_img, 0); //����Ҷ�ͼ��
	dstImg = Mat::zeros(srcImg.size(), CV_8UC1);
	yx_adaptiveThreshold(srcImg, binary_img, 7, -3);//��ֵ��
	//threshold(srcImg, b2, 150,255, CV_THRESH_BINARY );
	//imshow("only b", binary_img);
	//imshow("�̶���ֵ", b2);
	RemoveSmallRegion(binary_img, dstImg, 10, 0, 0);//ȥ�ڵ�
	RemoveSmallRegion(dstImg, dstImg, 10, 1, 0);//ȥ�׵�


}
/*
function��ͼ����ȥ��С�����Ż���ֵ����ͼ��
para��
CheckMode: 0����ȥ��������1����ȥ��������;
NeihborMode��0����4����1����8����;
AreaLimit��С�����������

ע�⣺dst������һ����ʼ�����Mat����С����Ϊ0
	 dst����Ϊsrc

*/

void RemoveSmallRegion(Mat& Src, Mat& Dst, int AreaLimit, int CheckMode, int NeihborMode)
{
	int RemoveCount = 0;       //��¼��ȥ�ĸ���  
							   //��¼ÿ�����ص����״̬�ı�ǩ��0����δ��飬1�������ڼ��,2�����鲻�ϸ���Ҫ��ת��ɫ����3������ϸ������  
	Mat Pointlabel = Mat::zeros(Src.size(), CV_8UC1);

	if (CheckMode == 1)
	{
		//cout << "Mode: ȥ��С����. ";
		for (int i = 0; i < Src.rows; ++i)
		{
			uchar* iData = Src.ptr<uchar>(i);
			uchar* iLabel = Pointlabel.ptr<uchar>(i);
			for (int j = 0; j < Src.cols; ++j)
			{
				if (iData[j] < 10)
				{
					iLabel[j] = 3;
				}
			}
		}
	}
	else
	{
		//cout << "Mode: ȥ���׶�. ";
		for (int i = 0; i < Src.rows; ++i)
		{
			uchar* iData = Src.ptr<uchar>(i);
			uchar* iLabel = Pointlabel.ptr<uchar>(i);
			for (int j = 0; j < Src.cols; ++j)
			{
				if (iData[j] > 10)
				{
					iLabel[j] = 3;
				}
			}
		}
	}

	vector<Point2i> NeihborPos;  //��¼�����λ��  
	NeihborPos.push_back(Point2i(-1, 0));
	NeihborPos.push_back(Point2i(1, 0));
	NeihborPos.push_back(Point2i(0, -1));
	NeihborPos.push_back(Point2i(0, 1));
	if (NeihborMode == 1)
	{
		//cout << "Neighbor mode: 8����." << endl;
		NeihborPos.push_back(Point2i(-1, -1));
		NeihborPos.push_back(Point2i(-1, 1));
		NeihborPos.push_back(Point2i(1, -1));
		NeihborPos.push_back(Point2i(1, 1));
	}
	//else cout << "Neighbor mode: 4����." << endl;
	int NeihborCount = 4 + 4 * NeihborMode;
	int CurrX = 0, CurrY = 0;
	//��ʼ���  
	for (int i = 0; i < Src.rows; ++i)
	{
		uchar* iLabel = Pointlabel.ptr<uchar>(i);
		for (int j = 0; j < Src.cols; ++j)
		{
			if (iLabel[j] == 0)
			{
				//********��ʼ�õ㴦�ļ��**********  
				vector<Point2i> GrowBuffer;                                      //��ջ�����ڴ洢������  
				GrowBuffer.push_back(Point2i(j, i));
				Pointlabel.at<uchar>(i, j) = 1;
				int CheckResult = 0;                                               //�����жϽ�����Ƿ񳬳���С����0Ϊδ������1Ϊ����  

				for (int z = 0; z<GrowBuffer.size(); z++)
				{

					for (int q = 0; q<NeihborCount; q++)                                      //����ĸ������  
					{
						CurrX = GrowBuffer.at(z).x + NeihborPos.at(q).x;
						CurrY = GrowBuffer.at(z).y + NeihborPos.at(q).y;
						if (CurrX >= 0 && CurrX<Src.cols&&CurrY >= 0 && CurrY<Src.rows)  //��ֹԽ��  
						{
							if (Pointlabel.at<uchar>(CurrY, CurrX) == 0)
							{
								GrowBuffer.push_back(Point2i(CurrX, CurrY));  //��������buffer  
								Pointlabel.at<uchar>(CurrY, CurrX) = 1;           //���������ļ���ǩ�������ظ����  
							}
						}
					}

				}
				if (GrowBuffer.size()>AreaLimit) CheckResult = 2;                 //�жϽ�����Ƿ񳬳��޶��Ĵ�С����1Ϊδ������2Ϊ����  
				else { CheckResult = 1;   RemoveCount++; }
				for (int z = 0; z<GrowBuffer.size(); z++)                         //����Label��¼  
				{
					CurrX = GrowBuffer.at(z).x;
					CurrY = GrowBuffer.at(z).y;
					Pointlabel.at<uchar>(CurrY, CurrX) += CheckResult;
				}
				//********�����õ㴦�ļ��**********  


			}
		}
	}

	CheckMode = 255 * (1 - CheckMode);
	//��ʼ��ת�����С������  
	for (int i = 0; i < Src.rows; ++i)
	{
		uchar* iData = Src.ptr<uchar>(i);
		uchar* iDstData = Dst.ptr<uchar>(i);
		uchar* iLabel = Pointlabel.ptr<uchar>(i);
		for (int j = 0; j < Src.cols; ++j)
		{
			if (iLabel[j] == 2)
			{
				iDstData[j] = CheckMode;
			}
			else if (iLabel[j] == 3)
			{
				iDstData[j] = iData[j];
			}
		}
	}

	//cout << RemoveCount << " objects removed." << endl;
}
/*
blockSize:��������ÿ�����ص���ֵ�������С����3,5,7�ȡ�
C:��adaptiveMethodѡ��ķ����м��������ƽ��ֵ���Ȩƽ��ֵ��ȥ�Ĳ���ֵ������������������
  CΪ������C����ֵ�������ͼ��׵�
  CΪ������C�������ͼ��ڵ�
*/
void yx_adaptiveThreshold(InputArray src, OutputArray dst, int blockSize, double C)
{
	adaptiveThreshold(src, dst, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, blockSize, C);
}

