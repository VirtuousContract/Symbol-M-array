#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <sstream>
using namespace cv;
using namespace std;

/*
坐标法

1：符号少（4），窗口大（7*7）
2：符号多（9），窗口小（5*5）
3:

以下为情况 1 ：
序列基元数：2
序列窗口： 7
序列长度： 2^7-1=127

阵列窗口： 7*7十字形
阵列尺寸： 127*127
符号个数：4
符号类型：箭头--->画图？？？
符号与数字arr_2d对应关系：
（0,0）2：上
（0,1）3：下
（1,0）4：左
（1,1）5：右
*/
/*数字阵列部分*/

#define window_mode 5
/* 7*7阵列 */
//#define array_size 127 
//#define window_size 7
//#define element_num 2

/* 5*5阵列 */
#define array_size 31 
#define window_size 5
#define element_num 2

/*绘图部分*/
#define base_len_pixel 10 //基元边长 单位：像素

int Arr[array_size];
int arr_2d[array_size][array_size];


Mat matrix_num2sym(vector<Mat> maskImage, int Arr_2d[][array_size]);
void array_generator();
void drawing();

int main()
{

	//下列函数调用顺序不能变
	array_generator();
	/*调试*/
	for (size_t i = 0; i < array_size; i++)
	{
	cout << Arr[i];
	}
	cout << endl;
	/*调试*/
	//for (size_t i = 0; i < array_size; i++)
	//{
	//	for (size_t j = 0; j < array_size; j++)
	//	{
	//		cout << arr_2d[i][j];
	//	}
	//	cout << endl;
	//}
	/*调试*/


	drawing();

	/*调试*/
	//打印result_slice到txt
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
	/*调试*/
	//system("pause");
	waitKey(0);
	return 0;
}



/*序列生成器*/
void array_generator()
{
	//ofstream SaveFile("matrix.txt");
	for (int i = 0; i < window_size; i++)  //序列发生器初值  1
	{
		Arr[i] = 1;
		//SaveFile << Arr[i];
	}

	//生成一维序列
	for (int i = 0; i < array_size - window_size; i++)
	{

		//递推关系式
		//3元关系式 //Arr[i + 9] = (6 - (Arr[i + 7] + Arr[i + 5] + Arr[i])) % element_num;    //这里的  6  为手工计算
		//cout << Arr[i + 4] << " ";
		//SaveFile << Arr[i + 9];

		//窗口为 7 时的递推关系式
		if (window_mode == 7)
		{
			Arr[i + window_size] = (Arr[i + 1] + Arr[i]) % element_num;
		}
		if (window_mode == 5)
		{
			//窗口为 5 时的递推关系式
			Arr[i + window_size] = (Arr[i + 2] + Arr[i]) % element_num;
		}


	}

	//SaveFile.close();

	//生成数字阵列 arr_2d
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
	//设定roi区域
	//计算左上角坐标
	Mat srcImage = Mat::zeros(array_size*base_len_pixel, array_size*base_len_pixel, CV_8UC3); //全黑空图

	Mat mask;

	int x = 0;
	int y = 0;
	for (int i = 0; i < array_size; i++)
	{
		for (int j = 0; j < array_size; j++)
		{
			//确定基元左上角坐标
			x = j * base_len_pixel;		//基元边长 单位：像素
			y = i * base_len_pixel;
			//确定基元类型
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
绘制基元，保存为JPG文件
读取基元图片到Mat中
*/
void drawing()
{
	Mat img_matrix = Mat::zeros(array_size*base_len_pixel, array_size*base_len_pixel, CV_8UC3); //全黑空图
																								//读取基元
	Mat base0 = imread("base_1.bmp"); //2
	Mat base1 = imread("base_2.bmp");//3
	Mat base2 = imread("base_3.bmp");//4
	Mat base3 = imread("base_4.bmp");//5
									 //基元图案存入容器 按顺序对应基元序号0 1 2...
	vector<Mat> base_img;  //掩膜
	base_img.push_back(base0);
	base_img.push_back(base1);
	base_img.push_back(base2);
	base_img.push_back(base3);

	img_matrix = matrix_num2sym(base_img, arr_2d);

	imshow("img_matrix", img_matrix);
	imwrite("img_matrix.jpg", img_matrix);
}