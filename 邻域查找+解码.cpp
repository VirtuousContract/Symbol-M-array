#include "opencv2/opencv.hpp"
#include "符号阵列.h"
using namespace std;
using namespace cv;

#define PatternCols 31
#define PatternRows 31
#define WinSize 5 //窗口大小
#define MaxVote 5 //投票法最大票数

void NeighbourhoodFind(vector<Base> &base_assemble)
{
	int row_offset_1 = 3;//从上/下多少行开始寻找邻域
	//int row_offset_2 = 3;
	//↓查找一级邻域
	for (size_t TargetBaseIndex = 0; TargetBaseIndex < base_assemble.size(); TargetBaseIndex++) //遍历所有基元,要找所有基元的邻域
	{
		//遍历范围，邻域查找范围:目标基元序号向前3*31个~向后3*31个
		int N_begin = (int)TargetBaseIndex - row_offset_1 * PatternCols;
		int N_end = (int)TargetBaseIndex + row_offset_1 * PatternCols ;
		int SearchDomainSize = N_end - N_begin;
		//防止越界
		if (N_begin<0) N_begin = 0;
		if (N_end>base_assemble.size()) N_end = (int)base_assemble.size();
		
		vector<NbhCandidate> AboveNeighbourhoodCandidate;//上邻域候选集合，数组元素为上邻域候选基元序号
		vector<NbhCandidate> BelowNeighbourhoodCandidate;
		vector<NbhCandidate> LeftNeighbourhoodCandidate;
		vector<NbhCandidate> RightNeighbourhoodCandidate;
		//下面的循环将搜索域内所有基元分为4类：上/下/左/右邻域候选基元
		for (size_t CurrentCheckBaseIndex = N_begin; CurrentCheckBaseIndex < N_end; CurrentCheckBaseIndex++)//遍历搜索域内基元 i:当前检测基元序号
		{
			//计算x坐标差值，y坐标差值，，当前检测基元-目标基元，，坐标单位：像素
			int DX; //x方向坐标差
			int DY; //y方向坐标差
			NbhCandidate nbhcan;
			DX = base_assemble[CurrentCheckBaseIndex].rect_center_coor.x - base_assemble[TargetBaseIndex].rect_center_coor.x;
			DY = base_assemble[CurrentCheckBaseIndex].rect_center_coor.y - base_assemble[TargetBaseIndex].rect_center_coor.y;

			//根据DX，DY判断当前基元属于哪类候选基元
			if (DX>0&&abs(DX)>=abs(DY)) //右邻域候选
			{
				nbhcan.index = CurrentCheckBaseIndex;
				nbhcan.DL = abs(DX) + abs(DY);
				RightNeighbourhoodCandidate.push_back(nbhcan);
			}
			else if (DX<0&&abs(DX)>=abs(DY))
			{
				nbhcan.index = (int)CurrentCheckBaseIndex;
				nbhcan.DL = abs(DX) + abs(DY);
				LeftNeighbourhoodCandidate.push_back(nbhcan);
			}
			else if (DY>0 && abs(DX) < abs(DY))
			{
				nbhcan.index = (int)CurrentCheckBaseIndex;
				nbhcan.DL = abs(DX) + abs(DY);
				AboveNeighbourhoodCandidate.push_back(nbhcan);
			}
			else if (DY < 0 && abs(DX) < abs(DY))
			{
				nbhcan.index = (int)CurrentCheckBaseIndex;
				nbhcan.DL = abs(DX) + abs(DY);
				BelowNeighbourhoodCandidate.push_back(nbhcan);
			}
			else if (DX==0&&DY==0) //说明检索到了自身
			{
				//不进行操作
			}
			else
			{
				cout << "基元邻域分类出错 line：65，邻域查找.cpp";
			}

		}

		//在邻域候选集合里找到距离最小的基元，既是所求邻域
		//找上邻域
		if (AboveNeighbourhoodCandidate.size() > 0)
		{
			//从vector<NbhCandidate> AboveNeighbourhoodCandidate;里找到DL最小的NbhCandidate对象，
			//然后将其index赋给base_assemble[TargetBaseIndex].AboveNbh
			int minDL=AboveNeighbourhoodCandidate[0].DL;
			int index= AboveNeighbourhoodCandidate[0].index;
			for (size_t i = 0; i < AboveNeighbourhoodCandidate.size(); i++)
			{
				if (AboveNeighbourhoodCandidate[i].DL<minDL)
				{
					minDL = AboveNeighbourhoodCandidate[i].DL;
					index = AboveNeighbourhoodCandidate[i].index;
				}
			}
			base_assemble[TargetBaseIndex].AboveNbh = index;
		}
		else 
		{
			//说明上邻域不存在，则不进行任何操作，保留其默认值-1
		}

		//找下邻域
		if (BelowNeighbourhoodCandidate.size() > 0)
		{
			//从vector<NbhCandidate> BelowNeighbourhoodCandidate;里找到DL最小的NbhCandidate对象，
			//然后将其index赋给base_assemble[TargetBaseIndex].BelowNbh
			int minDL = BelowNeighbourhoodCandidate[0].DL;
			int index = BelowNeighbourhoodCandidate[0].index;
			for (size_t i = 0; i < BelowNeighbourhoodCandidate.size(); i++)
			{
				if (BelowNeighbourhoodCandidate[i].DL<minDL)
				{
					minDL = BelowNeighbourhoodCandidate[i].DL;
					index = BelowNeighbourhoodCandidate[i].index;
				}
			}
			base_assemble[TargetBaseIndex].BelowNbh = index;
		}
		else
		{
			//说明下邻域不存在，则不进行任何操作，保留其默认值-1
		}

		//找左邻域
		if (LeftNeighbourhoodCandidate.size() > 0)
		{
			//从vector<NbhCandidate> LeftNeighbourhoodCandidate;里找到DL最小的NbhCandidate对象，
			//然后将其index赋给base_assemble[TargetBaseIndex].LeftNbh
			int minDL = LeftNeighbourhoodCandidate[0].DL;
			int index = LeftNeighbourhoodCandidate[0].index;
			for (size_t i = 0; i < LeftNeighbourhoodCandidate.size(); i++)
			{
				if (LeftNeighbourhoodCandidate[i].DL<minDL)
				{
					minDL = LeftNeighbourhoodCandidate[i].DL;
					index = LeftNeighbourhoodCandidate[i].index;
				}
			}
			base_assemble[TargetBaseIndex].LeftNbh = index;
		}
		else
		{
			//说明左邻域不存在，则不进行任何操作，保留其默认值-1
		}

		//找右邻域
		if (RightNeighbourhoodCandidate.size() > 0)
		{
			//从vector<NbhCandidate> RightNeighbourhoodCandidate;里找到DL最小的NbhCandidate对象，
			//然后将其index赋给base_assemble[TargetBaseIndex].RightNbh
			int minDL = RightNeighbourhoodCandidate[0].DL;
			int index = RightNeighbourhoodCandidate[0].index;
			for (size_t i = 0; i < RightNeighbourhoodCandidate.size(); i++)
			{
				if (RightNeighbourhoodCandidate[i].DL<minDL)
				{
					minDL = RightNeighbourhoodCandidate[i].DL;
					index = RightNeighbourhoodCandidate[i].index;
				}
			}
			base_assemble[TargetBaseIndex].RightNbh = index;
		}
		else
		{
			//说明右邻域不存在，则不进行任何操作，保留其默认值-1
		}


	}
	

	//↑所有基元一级邻域已经找到 //一级邻域实验验证效果良好
	//下面通过四邻域找到5*5的邻域，并标记出边界1和边界2
	for (size_t TargetBaseIndex = 0; TargetBaseIndex < base_assemble.size(); TargetBaseIndex++)
	{
		int above = base_assemble[TargetBaseIndex].AboveNbh;
		int below = base_assemble[TargetBaseIndex].BelowNbh;
		int left = base_assemble[TargetBaseIndex].LeftNbh;
		int right = base_assemble[TargetBaseIndex].RightNbh;

		int above2;
		int below2;
		int left2;
		int right2;

		//目标基元的二级邻域序号
		if (above>=0)
		{
			above2 = base_assemble[above].AboveNbh;
		}
		else
		{
			above2 = -1;
		}
		if (below>=0)
		{
			below2 = base_assemble[below].BelowNbh;
		}
		else
		{
			below2 = -1;
		}
		if (left>=0)
		{
			left2 = base_assemble[left].LeftNbh;
		}
		else
		{
			left2 = 0;
		}
		if (right>=0)
		{
			right2 = base_assemble[right].RightNbh;
		}
		else
		{
			right2 = -1;
		}
		 
		base_assemble[TargetBaseIndex].AboveNbh2= above2;
		base_assemble[TargetBaseIndex].BelowNbh2 = below2;
		base_assemble[TargetBaseIndex].LeftNbh2 = left2;
		base_assemble[TargetBaseIndex].RightNbh2 = right2;
		//↑二级邻域查找完成

		//下面的代码，根据一/二级邻域的存在情况判断当前基元是否属于边界，及边界类型
		//与二级邻域查找共用一个循环，完成基元邻域查找之后立刻判断
		//同时确定码值来源，即将来通过那个基元计算自身码值（行列号）

		//判断是否为边界类型1   （边界判断不会与表面断裂处判断产生冲突）
		//边界1根据码值来源分为八种情况：上下左右四个角。 先判断四个角

		if (base_assemble[TargetBaseIndex].AboveNbh == -1 || base_assemble[TargetBaseIndex].LeftNbh == -1 
			|| base_assemble[TargetBaseIndex].BelowNbh == -1 && base_assemble[TargetBaseIndex].RightNbh == -1)
		{
			//一级邻域
			base_assemble[TargetBaseIndex].boundary_flag = 1;

			
		}

		else if(base_assemble[TargetBaseIndex].AboveNbh2 == -1 || base_assemble[TargetBaseIndex].LeftNbh2 == -1
			|| base_assemble[TargetBaseIndex].BelowNbh2 == -1 && base_assemble[TargetBaseIndex].RightNbh2 == -1)
		{
			base_assemble[TargetBaseIndex].boundary_flag = 2;

			
		}
		//↑边界判断标记完成
		else  //目标基元位于可解码区域
		{
			base_assemble[TargetBaseIndex].boundary_flag = 0;
			//base_assemble[TargetBaseIndex].CodewordSource = -1;
		}
		//↑ 所有基元边界标记完成，二级邻域查找完成
		//↓ 对可解码区域进行解码

	}
}

//解码
void Decode(vector<Base> &base_assemble)
{
	//先解码非边界区域
	//利用十字形邻域直接求出区域内所有基元的码值，在此基础上由投票法确定最终的码值。(原理同于参考论文)
	vector<Base> base_assemble_cache(base_assemble);//存储可解码区域 邻域直接解码的信息
	for (size_t TargetBaseIndex = 0; TargetBaseIndex < base_assemble_cache.size(); TargetBaseIndex++)
	{
		int RowCodeBase[WinSize];//存储计算行号的基元 从上到下
		int ColCodeBase[WinSize];//存储计算列号的基元 从左到右

		int RowBinaryCode[WinSize];//行号二进制码 存储用于计算行号的5位二进制数
		int ColBinaryCode[WinSize];//列号二进制码
		
		if (base_assemble_cache[TargetBaseIndex].boundary_flag==0)
		{
			RowCodeBase[0] = base_assemble_cache[TargetBaseIndex].AboveNbh2;
			RowCodeBase[1] = base_assemble_cache[TargetBaseIndex].AboveNbh;
			RowCodeBase[2] = TargetBaseIndex;
			RowCodeBase[3] = base_assemble_cache[TargetBaseIndex].BelowNbh;
			RowCodeBase[4] = base_assemble_cache[TargetBaseIndex].BelowNbh2;

			ColCodeBase[0] = base_assemble_cache[TargetBaseIndex].LeftNbh2;
			ColCodeBase[1] = base_assemble_cache[TargetBaseIndex].LeftNbh;
			ColCodeBase[2] = TargetBaseIndex;
			ColCodeBase[3] = base_assemble_cache[TargetBaseIndex].RightNbh;
			ColCodeBase[4] = base_assemble_cache[TargetBaseIndex].RightNbh2;

			//获得5位二进制格式 的行列号码值
			for (size_t i = 0; i < WinSize; i++)
			{
				RowBinaryCode[i] = (SymbolRecognition(base_assemble_cache[RowCodeBase[i]])).first;
				ColBinaryCode[i] = (SymbolRecognition(base_assemble_cache[ColCodeBase[i]])).second;
			}

			//由二进制码值计算行列号
			base_assemble_cache[TargetBaseIndex].codeword.RowNum = BinaryCodeToRowColNumber(RowBinaryCode);
			base_assemble_cache[TargetBaseIndex].codeword.ColNum = BinaryCodeToRowColNumber(ColBinaryCode);
		}
		/////////////////////////////////////////////
	}

	//投票法  
	//再次遍历可解码区域
	for (size_t TargetBaseIndex = 0; TargetBaseIndex < base_assemble.size(); TargetBaseIndex++)
	{
		//找到上下左右中五个基元
		int above = base_assemble[TargetBaseIndex].AboveNbh;
		int below = base_assemble[TargetBaseIndex].BelowNbh;
		int left = base_assemble[TargetBaseIndex].LeftNbh;
		int right = base_assemble[TargetBaseIndex].RightNbh;
		int target = TargetBaseIndex;

		//获得 个行号 个列号，最大为5个，如果5个里存在边界基元，则排除；
		vector<int> RowCandidate; //参与投票的行号
		vector<int> ColCandidate; //参与投票的列号
		if (base_assemble[above].boundary_flag==0) //具有投票资格
		{
			RowCandidate.push_back(base_assemble_cache[above].codeword.RowNum);
			ColCandidate.push_back(base_assemble_cache[above].codeword.ColNum);
		}
		if (base_assemble[below].boundary_flag == 0)
		{
			RowCandidate.push_back(base_assemble_cache[below].codeword.RowNum);
			ColCandidate.push_back(base_assemble_cache[below].codeword.ColNum);
		}
		if (base_assemble[left].boundary_flag == 0)
		{
			RowCandidate.push_back(base_assemble_cache[left].codeword.RowNum);
			ColCandidate.push_back(base_assemble_cache[left].codeword.ColNum);
		}
		if (base_assemble[right].boundary_flag == 0)
		{
			RowCandidate.push_back(base_assemble_cache[right].codeword.RowNum);
			ColCandidate.push_back(base_assemble_cache[right].codeword.ColNum);
		}
		if (base_assemble[target].boundary_flag == 0)
		{
			RowCandidate.push_back(base_assemble_cache[target].codeword.RowNum);
			ColCandidate.push_back(base_assemble_cache[target].codeword.ColNum);
		}

		//投票：选出出现次数最多的行、列号
		//出现次数最多的作为最终结果，暂不考虑票数达到多少才合格

	}
	//再解码边界2
	for (size_t TargetBaseIndex = 0; TargetBaseIndex < base_assemble.size(); TargetBaseIndex++)
	{
		if (base_assemble[TargetBaseIndex].boundary_flag == 2)
		{
			if (base_assemble[TargetBaseIndex].AboveNbh2 == -1 && base_assemble[TargetBaseIndex].LeftNbh2 == -1)
			{
				//左上角
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].RightNbh].BelowNbh].codeword.ColNum + 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].RightNbh].BelowNbh].codeword.RowNum + 1;
			}
			else if (base_assemble[TargetBaseIndex].AboveNbh2 == -1 && base_assemble[TargetBaseIndex].RightNbh2 == -1)
			{
				//右上角
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].BelowNbh].codeword.ColNum - 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].BelowNbh].codeword.RowNum + 1;
			}
			else if (base_assemble[TargetBaseIndex].BelowNbh2 == -1 && base_assemble[TargetBaseIndex].RightNbh2 == -1)
			{
				//右下角
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].AboveNbh].codeword.ColNum - 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].AboveNbh].codeword.RowNum - 1;
			}
			else if (base_assemble[TargetBaseIndex].BelowNbh2 == -1 && base_assemble[TargetBaseIndex].LeftNbh2 == -1)
			{
				//左下角
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].RightNbh].AboveNbh].codeword.ColNum + 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].RightNbh].AboveNbh].codeword.RowNum - 1;
			}
			else if (base_assemble[TargetBaseIndex].AboveNbh2 == -1)
			{
				//base_assemble[TargetBaseIndex].CodewordSource = base_assemble[TargetBaseIndex].BelowNbh;
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[TargetBaseIndex].BelowNbh].codeword.ColNum;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[TargetBaseIndex].BelowNbh].codeword.RowNum + 1;
			}
			else if (base_assemble[TargetBaseIndex].BelowNbh2 == -1)
			{
				//base_assemble[TargetBaseIndex].CodewordSource = base_assemble[TargetBaseIndex].AboveNbh;
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[TargetBaseIndex].AboveNbh].codeword.ColNum;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[TargetBaseIndex].AboveNbh].codeword.RowNum - 1;
			}
			else if (base_assemble[TargetBaseIndex].LeftNbh2 == -1)
			{
				//base_assemble[TargetBaseIndex].CodewordSource = base_assemble[TargetBaseIndex].RightNbh;
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[TargetBaseIndex].RightNbh].codeword.ColNum + 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[TargetBaseIndex].RightNbh].codeword.RowNum;
			}
			else if (base_assemble[TargetBaseIndex].RightNbh2 == -1)
			{
				//base_assemble[TargetBaseIndex].CodewordSource = base_assemble[TargetBaseIndex].LeftNbh;
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[TargetBaseIndex].LeftNbh].codeword.ColNum - 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[TargetBaseIndex].LeftNbh].codeword.RowNum;
			}
		}
	}
	//最后解码边界1
	for (size_t TargetBaseIndex = 0; TargetBaseIndex < base_assemble.size(); TargetBaseIndex++)
	{
		if (base_assemble[TargetBaseIndex].boundary_flag == 1)
		{
			if (base_assemble[TargetBaseIndex].AboveNbh == -1 && base_assemble[TargetBaseIndex].LeftNbh == -1)
			{
				//左上角
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].RightNbh].BelowNbh].codeword.ColNum + 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].RightNbh].BelowNbh].codeword.RowNum + 1;
			}
			else if (base_assemble[TargetBaseIndex].AboveNbh == -1 && base_assemble[TargetBaseIndex].RightNbh == -1)
			{
				//右上角
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].BelowNbh].codeword.ColNum - 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].BelowNbh].codeword.RowNum + 1;
			}
			else if (base_assemble[TargetBaseIndex].BelowNbh == -1 && base_assemble[TargetBaseIndex].RightNbh == -1)
			{
				//右下角
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].AboveNbh].codeword.ColNum - 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].AboveNbh].codeword.RowNum - 1;
			}
			else if (base_assemble[TargetBaseIndex].BelowNbh == -1 && base_assemble[TargetBaseIndex].LeftNbh == -1)
			{
				//左下角
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].RightNbh].AboveNbh].codeword.ColNum + 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].RightNbh].AboveNbh].codeword.RowNum - 1;
			}
			else if (base_assemble[TargetBaseIndex].AboveNbh == -1)
			{
				//base_assemble[TargetBaseIndex].CodewordSource = base_assemble[TargetBaseIndex].BelowNbh;
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[TargetBaseIndex].BelowNbh].codeword.ColNum;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[TargetBaseIndex].BelowNbh].codeword.RowNum + 1;
			}
			else if (base_assemble[TargetBaseIndex].BelowNbh == -1)
			{
				//base_assemble[TargetBaseIndex].CodewordSource = base_assemble[TargetBaseIndex].AboveNbh;
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[TargetBaseIndex].AboveNbh].codeword.ColNum;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[TargetBaseIndex].AboveNbh].codeword.RowNum - 1;
			}
			else if (base_assemble[TargetBaseIndex].LeftNbh == -1)
			{
				//base_assemble[TargetBaseIndex].CodewordSource = base_assemble[TargetBaseIndex].RightNbh;
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[TargetBaseIndex].RightNbh].codeword.ColNum + 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[TargetBaseIndex].RightNbh].codeword.RowNum;
			}
			else if (base_assemble[TargetBaseIndex].RightNbh == -1)
			{
				//base_assemble[TargetBaseIndex].CodewordSource = base_assemble[TargetBaseIndex].LeftNbh;
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[TargetBaseIndex].LeftNbh].codeword.ColNum - 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[TargetBaseIndex].LeftNbh].codeword.RowNum;
			}
		}
	}
}

/*
	功能：查找二进制码值对应的一维序列位置，行列号从0开始计数
	input:5位二进制码值
	output：行列号，码值所表示的基元在一维序列中的位置
   如果返回值==-999：匹配失败，说明基元识别错误
   准确的返回值应在范围内：2~28
*/
int BinaryCodeToRowColNumber(int input[])
{
	//一维伪随机序列
	int Sequence[31] = { 1,1,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0,1,0,0,0,0,1,0,0,1,0,1,1,0,0 };
	int RowColNumber = -999;
	for (size_t i = 0; i < 27; i++) //遍历Sequence【31】
	{
		if (Sequence[i]==input[0]&&Sequence[i+1]==input[1]&&Sequence[i+2]==input[2]
			&&Sequence[i+3]==input[3]&&Sequence[i+4]==input[4])
		{
			RowColNumber = i + 2; //行列号为i+2
			break;
		}
		
	}

	return RowColNumber;
}
