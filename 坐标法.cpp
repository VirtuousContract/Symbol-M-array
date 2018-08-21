#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>
using namespace cv;
using namespace std;

/*
���귨

1�������٣�4�������ڴ�7*7��
2�����Ŷࣨ9��������С��5*5��
3:

����Ϊ��� 1 ��
���л�Ԫ����2
���д��ڣ� 7
���г��ȣ� 2^7-1=127

���д��ڣ� 7*7ʮ����
���гߴ磺 127*127
���Ÿ�����4
�������ͣ���ͷ--->��ͼ������
����������arr_2d��Ӧ��ϵ��
��0,0��2����
��0,1��3����
��1,0��4����
��1,1��5����
*/
/*�������в���*/

#define window_mode 5
/* 7*7���� */
//#define array_size 127 
//#define window_size 7
//#define element_num 2

/* 5*5���� */
#define array_size 31 
#define window_size 5
#define element_num 2

/*��ͼ����*/
#define base_len_pixel 10 //��Ԫ�߳� ��λ������

int Arr[array_size];
int arr_2d[array_size][array_size];


Mat matrix_num2sym(vector<Mat> maskImage, int Arr_2d[][array_size]);
void array_generator();
void drawing();

int main()
{

	//���к�������˳���ܱ�
	array_generator();
	/*����*/
	for (size_t i = 0; i < array_size; i++)
	{
	cout << Arr[i];
	}
	cout << endl;
	/*����*/
	//for (size_t i = 0; i < array_size; i++)
	//{
	//	for (size_t j = 0; j < array_size; j++)
	//	{
	//		cout << arr_2d[i][j];
	//	}
	//	cout << endl;
	//}
	/*����*/


	drawing();

	/*����*/
	//��ӡresult_slice��txt
	ofstream SaveFile("result_matrix_127.txt");
	for (size_t i = 0; i < array_size; i++)
	{
		for (size_t j = 0; j < array_size; j++)
		{
			SaveFile << arr_2d[i][j];
		}
		SaveFile << endl;
	}

	SaveFile.close();
	/*����*/
	//system("pause");
	waitKey(0);
	return 0;
}



/*����������*/
void array_generator()
{
	//ofstream SaveFile("matrix.txt");
	for (int i = 0; i < window_size; i++)  //���з�������ֵ  1
	{
		Arr[i] = 1;
		//SaveFile << Arr[i];
	}

	//����һά����
	for (int i = 0; i < array_size - window_size; i++)
	{

		//���ƹ�ϵʽ
		//3Ԫ��ϵʽ //Arr[i + 9] = (6 - (Arr[i + 7] + Arr[i + 5] + Arr[i])) % element_num;    //�����  6  Ϊ�ֹ�����
		//cout << Arr[i + 4] << " ";
		//SaveFile << Arr[i + 9];

		//����Ϊ 7 ʱ�ĵ��ƹ�ϵʽ
		if (window_mode == 7)
		{
			Arr[i + window_size] = (Arr[i + 1] + Arr[i]) % element_num;
		}
		if (window_mode == 5)
		{
			//����Ϊ 5 ʱ�ĵ��ƹ�ϵʽ
			Arr[i + window_size] = (Arr[i + 2] + Arr[i]) % element_num;
		}


	}

	//SaveFile.close();

	//������������ arr_2d
	for (size_t i = 0; i < array_size; i++)
	{
		for (size_t j = 0; j < array_size; j++)
		{
			if (Arr[i] == 0 && Arr[ j] == 0)
			{
				arr_2d[i][j] = 2;
			}
			else if (Arr[i] == 0 && Arr[ j] == 1)
			{
				arr_2d[i][j] = 3;
			}
			else if (Arr[i] == 1 && Arr[ j] == 0)
			{
				arr_2d[i][j] = 4;
			}
			else if (Arr[i] == 1 && Arr[ j] == 1)
			{
				arr_2d[i][j] = 5;
			}
		}
	}



}

Mat matrix_num2sym(vector<Mat> maskImage, int Arr_2d[][array_size])
{
	//�趨roi����
	//�������Ͻ�����
	Mat srcImage = Mat::zeros(array_size*base_len_pixel, array_size*base_len_pixel, CV_8UC3); //ȫ�ڿ�ͼ

	Mat mask;

	int x = 0;
	int y = 0;
	for (int i = 0; i < array_size; i++)
	{
		for (int j = 0; j < array_size; j++)
		{
			//ȷ����Ԫ���Ͻ�����
			x = j * base_len_pixel;		//��Ԫ�߳� ��λ������
			y = i * base_len_pixel;
			//ȷ����Ԫ����
			switch (Arr_2d[i][j])
			{
			case 2:
				mask = maskImage[0];
				break;
			case 3:
				mask = maskImage[1];
				break;
			case 4:
				mask = maskImage[2];
				break;
			case 5:
				mask = maskImage[3];
				break;
			default:
				break;
			}
			Mat imageROI = srcImage(Rect(x, y, mask.cols, mask.rows));
			mask.copyTo(imageROI, mask);
		}
	}
	return srcImage;

}

/*
���ƻ�Ԫ������ΪJPG�ļ�
��ȡ��ԪͼƬ��Mat��
*/
void drawing()
{
	Mat img_matrix = Mat::zeros(array_size*base_len_pixel, array_size*base_len_pixel, CV_8UC3); //ȫ�ڿ�ͼ
																								//��ȡ��Ԫ
	Mat base0 = imread("base_1.bmp"); //2
	Mat base1 = imread("base_2.bmp");//3
	Mat base2 = imread("base_3.bmp");//4
	Mat base3 = imread("base_4.bmp");//5
									 //��Ԫͼ���������� ��˳���Ӧ��Ԫ���0 1 2...
	vector<Mat> base_img;  //��Ĥ
	base_img.push_back(base0);
	base_img.push_back(base1);
	base_img.push_back(base2);
	base_img.push_back(base3);

	img_matrix = matrix_num2sym(base_img, arr_2d);

	imshow("img_matrix", img_matrix);
	imwrite("img_matrix.jpg", img_matrix);
}