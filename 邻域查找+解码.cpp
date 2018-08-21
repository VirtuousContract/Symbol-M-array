#include "opencv2/opencv.hpp"
#include "��������.h"
using namespace std;
using namespace cv;

#define PatternCols 31
#define PatternRows 31
#define WinSize 5 //���ڴ�С
#define MaxVote 5 //ͶƱ�����Ʊ��

void NeighbourhoodFind(vector<Base> &base_assemble)
{
	int row_offset_1 = 3;//����/�¶����п�ʼѰ������
	//int row_offset_2 = 3;
	//������һ������
	for (size_t TargetBaseIndex = 0; TargetBaseIndex < base_assemble.size(); TargetBaseIndex++) //�������л�Ԫ,Ҫ�����л�Ԫ������
	{
		//������Χ��������ҷ�Χ:Ŀ���Ԫ�����ǰ3*31��~���3*31��
		int N_begin = (int)TargetBaseIndex - row_offset_1 * PatternCols;
		int N_end = (int)TargetBaseIndex + row_offset_1 * PatternCols ;
		int SearchDomainSize = N_end - N_begin;
		//��ֹԽ��
		if (N_begin<0) N_begin = 0;
		if (N_end>base_assemble.size()) N_end = (int)base_assemble.size();
		
		vector<NbhCandidate> AboveNeighbourhoodCandidate;//�������ѡ���ϣ�����Ԫ��Ϊ�������ѡ��Ԫ���
		vector<NbhCandidate> BelowNeighbourhoodCandidate;
		vector<NbhCandidate> LeftNeighbourhoodCandidate;
		vector<NbhCandidate> RightNeighbourhoodCandidate;
		//�����ѭ���������������л�Ԫ��Ϊ4�ࣺ��/��/��/�������ѡ��Ԫ
		for (size_t CurrentCheckBaseIndex = N_begin; CurrentCheckBaseIndex < N_end; CurrentCheckBaseIndex++)//�����������ڻ�Ԫ i:��ǰ����Ԫ���
		{
			//����x�����ֵ��y�����ֵ������ǰ����Ԫ-Ŀ���Ԫ�������굥λ������
			int DX; //x���������
			int DY; //y���������
			NbhCandidate nbhcan;
			DX = base_assemble[CurrentCheckBaseIndex].rect_center_coor.x - base_assemble[TargetBaseIndex].rect_center_coor.x;
			DY = base_assemble[CurrentCheckBaseIndex].rect_center_coor.y - base_assemble[TargetBaseIndex].rect_center_coor.y;

			//����DX��DY�жϵ�ǰ��Ԫ���������ѡ��Ԫ
			if (DX>0&&abs(DX)>=abs(DY)) //�������ѡ
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
			else if (DX==0&&DY==0) //˵��������������
			{
				//�����в���
			}
			else
			{
				cout << "��Ԫ���������� line��65���������.cpp";
			}

		}

		//�������ѡ�������ҵ�������С�Ļ�Ԫ��������������
		//��������
		if (AboveNeighbourhoodCandidate.size() > 0)
		{
			//��vector<NbhCandidate> AboveNeighbourhoodCandidate;���ҵ�DL��С��NbhCandidate����
			//Ȼ����index����base_assemble[TargetBaseIndex].AboveNbh
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
			//˵�������򲻴��ڣ��򲻽����κβ�����������Ĭ��ֵ-1
		}

		//��������
		if (BelowNeighbourhoodCandidate.size() > 0)
		{
			//��vector<NbhCandidate> BelowNeighbourhoodCandidate;���ҵ�DL��С��NbhCandidate����
			//Ȼ����index����base_assemble[TargetBaseIndex].BelowNbh
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
			//˵�������򲻴��ڣ��򲻽����κβ�����������Ĭ��ֵ-1
		}

		//��������
		if (LeftNeighbourhoodCandidate.size() > 0)
		{
			//��vector<NbhCandidate> LeftNeighbourhoodCandidate;���ҵ�DL��С��NbhCandidate����
			//Ȼ����index����base_assemble[TargetBaseIndex].LeftNbh
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
			//˵�������򲻴��ڣ��򲻽����κβ�����������Ĭ��ֵ-1
		}

		//��������
		if (RightNeighbourhoodCandidate.size() > 0)
		{
			//��vector<NbhCandidate> RightNeighbourhoodCandidate;���ҵ�DL��С��NbhCandidate����
			//Ȼ����index����base_assemble[TargetBaseIndex].RightNbh
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
			//˵�������򲻴��ڣ��򲻽����κβ�����������Ĭ��ֵ-1
		}


	}
	

	//�����л�Ԫһ�������Ѿ��ҵ� //һ������ʵ����֤Ч������
	//����ͨ���������ҵ�5*5�����򣬲���ǳ��߽�1�ͱ߽�2
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

		//Ŀ���Ԫ�Ķ����������
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
		//����������������

		//����Ĵ��룬����һ/��������Ĵ�������жϵ�ǰ��Ԫ�Ƿ����ڱ߽磬���߽�����
		//�����������ҹ���һ��ѭ������ɻ�Ԫ�������֮�������ж�
		//ͬʱȷ����ֵ��Դ��������ͨ���Ǹ���Ԫ����������ֵ�����кţ�

		//�ж��Ƿ�Ϊ�߽�����1   ���߽��жϲ����������Ѵ��жϲ�����ͻ��
		//�߽�1������ֵ��Դ��Ϊ������������������ĸ��ǡ� ���ж��ĸ���

		if (base_assemble[TargetBaseIndex].AboveNbh == -1 || base_assemble[TargetBaseIndex].LeftNbh == -1 
			|| base_assemble[TargetBaseIndex].BelowNbh == -1 && base_assemble[TargetBaseIndex].RightNbh == -1)
		{
			//һ������
			base_assemble[TargetBaseIndex].boundary_flag = 1;

			
		}

		else if(base_assemble[TargetBaseIndex].AboveNbh2 == -1 || base_assemble[TargetBaseIndex].LeftNbh2 == -1
			|| base_assemble[TargetBaseIndex].BelowNbh2 == -1 && base_assemble[TargetBaseIndex].RightNbh2 == -1)
		{
			base_assemble[TargetBaseIndex].boundary_flag = 2;

			
		}
		//���߽��жϱ�����
		else  //Ŀ���Ԫλ�ڿɽ�������
		{
			base_assemble[TargetBaseIndex].boundary_flag = 0;
			//base_assemble[TargetBaseIndex].CodewordSource = -1;
		}
		//�� ���л�Ԫ�߽�����ɣ���������������
		//�� �Կɽ���������н���

	}
}

//����
void Decode(vector<Base> &base_assemble)
{
	//�Ƚ���Ǳ߽�����
	//����ʮ��������ֱ��������������л�Ԫ����ֵ���ڴ˻�������ͶƱ��ȷ�����յ���ֵ��(ԭ��ͬ�ڲο�����)
	vector<Base> base_assemble_cache(base_assemble);//�洢�ɽ������� ����ֱ�ӽ������Ϣ
	for (size_t TargetBaseIndex = 0; TargetBaseIndex < base_assemble_cache.size(); TargetBaseIndex++)
	{
		int RowCodeBase[WinSize];//�洢�����кŵĻ�Ԫ ���ϵ���
		int ColCodeBase[WinSize];//�洢�����кŵĻ�Ԫ ������

		int RowBinaryCode[WinSize];//�кŶ������� �洢���ڼ����кŵ�5λ��������
		int ColBinaryCode[WinSize];//�кŶ�������
		
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

			//���5λ�����Ƹ�ʽ �����к���ֵ
			for (size_t i = 0; i < WinSize; i++)
			{
				RowBinaryCode[i] = (SymbolRecognition(base_assemble_cache[RowCodeBase[i]])).first;
				ColBinaryCode[i] = (SymbolRecognition(base_assemble_cache[ColCodeBase[i]])).second;
			}

			//�ɶ�������ֵ�������к�
			base_assemble_cache[TargetBaseIndex].codeword.RowNum = BinaryCodeToRowColNumber(RowBinaryCode);
			base_assemble_cache[TargetBaseIndex].codeword.ColNum = BinaryCodeToRowColNumber(ColBinaryCode);
		}
		/////////////////////////////////////////////
	}

	//ͶƱ��  
	//�ٴα����ɽ�������
	for (size_t TargetBaseIndex = 0; TargetBaseIndex < base_assemble.size(); TargetBaseIndex++)
	{
		//�ҵ����������������Ԫ
		int above = base_assemble[TargetBaseIndex].AboveNbh;
		int below = base_assemble[TargetBaseIndex].BelowNbh;
		int left = base_assemble[TargetBaseIndex].LeftNbh;
		int right = base_assemble[TargetBaseIndex].RightNbh;
		int target = TargetBaseIndex;

		//��� ���к� ���кţ����Ϊ5�������5������ڱ߽��Ԫ�����ų���
		vector<int> RowCandidate; //����ͶƱ���к�
		vector<int> ColCandidate; //����ͶƱ���к�
		if (base_assemble[above].boundary_flag==0) //����ͶƱ�ʸ�
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

		//ͶƱ��ѡ�����ִ��������С��к�
		//���ִ���������Ϊ���ս�����ݲ�����Ʊ���ﵽ���ٲźϸ�

	}
	//�ٽ���߽�2
	for (size_t TargetBaseIndex = 0; TargetBaseIndex < base_assemble.size(); TargetBaseIndex++)
	{
		if (base_assemble[TargetBaseIndex].boundary_flag == 2)
		{
			if (base_assemble[TargetBaseIndex].AboveNbh2 == -1 && base_assemble[TargetBaseIndex].LeftNbh2 == -1)
			{
				//���Ͻ�
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].RightNbh].BelowNbh].codeword.ColNum + 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].RightNbh].BelowNbh].codeword.RowNum + 1;
			}
			else if (base_assemble[TargetBaseIndex].AboveNbh2 == -1 && base_assemble[TargetBaseIndex].RightNbh2 == -1)
			{
				//���Ͻ�
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].BelowNbh].codeword.ColNum - 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].BelowNbh].codeword.RowNum + 1;
			}
			else if (base_assemble[TargetBaseIndex].BelowNbh2 == -1 && base_assemble[TargetBaseIndex].RightNbh2 == -1)
			{
				//���½�
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].AboveNbh].codeword.ColNum - 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].AboveNbh].codeword.RowNum - 1;
			}
			else if (base_assemble[TargetBaseIndex].BelowNbh2 == -1 && base_assemble[TargetBaseIndex].LeftNbh2 == -1)
			{
				//���½�
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
	//������߽�1
	for (size_t TargetBaseIndex = 0; TargetBaseIndex < base_assemble.size(); TargetBaseIndex++)
	{
		if (base_assemble[TargetBaseIndex].boundary_flag == 1)
		{
			if (base_assemble[TargetBaseIndex].AboveNbh == -1 && base_assemble[TargetBaseIndex].LeftNbh == -1)
			{
				//���Ͻ�
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].RightNbh].BelowNbh].codeword.ColNum + 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].RightNbh].BelowNbh].codeword.RowNum + 1;
			}
			else if (base_assemble[TargetBaseIndex].AboveNbh == -1 && base_assemble[TargetBaseIndex].RightNbh == -1)
			{
				//���Ͻ�
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].BelowNbh].codeword.ColNum - 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].BelowNbh].codeword.RowNum + 1;
			}
			else if (base_assemble[TargetBaseIndex].BelowNbh == -1 && base_assemble[TargetBaseIndex].RightNbh == -1)
			{
				//���½�
				base_assemble[TargetBaseIndex].codeword.ColNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].AboveNbh].codeword.ColNum - 1;
				base_assemble[TargetBaseIndex].codeword.RowNum = base_assemble[base_assemble[base_assemble[TargetBaseIndex].LeftNbh].AboveNbh].codeword.RowNum - 1;
			}
			else if (base_assemble[TargetBaseIndex].BelowNbh == -1 && base_assemble[TargetBaseIndex].LeftNbh == -1)
			{
				//���½�
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
	���ܣ����Ҷ�������ֵ��Ӧ��һά����λ�ã����кŴ�0��ʼ����
	input:5λ��������ֵ
	output�����кţ���ֵ����ʾ�Ļ�Ԫ��һά�����е�λ��
   �������ֵ==-999��ƥ��ʧ�ܣ�˵����Ԫʶ�����
   ׼ȷ�ķ���ֵӦ�ڷ�Χ�ڣ�2~28
*/
int BinaryCodeToRowColNumber(int input[])
{
	//һάα�������
	int Sequence[31] = { 1,1,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0,1,0,0,0,0,1,0,0,1,0,1,1,0,0 };
	int RowColNumber = -999;
	for (size_t i = 0; i < 27; i++) //����Sequence��31��
	{
		if (Sequence[i]==input[0]&&Sequence[i+1]==input[1]&&Sequence[i+2]==input[2]
			&&Sequence[i+3]==input[3]&&Sequence[i+4]==input[4])
		{
			RowColNumber = i + 2; //���к�Ϊi+2
			break;
		}
		
	}

	return RowColNumber;
}
