#include "tabular.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

LogicExpr FileRead(const char file_name[])
{
	std::string bit_length;
	StringData temp;	//여기에 데이터를 저장해서 반환할 것임

	LogicExpr logic_expr;
	logic_expr.minterm_num = 0;
	logic_expr.bit_length = '0';
	std::ifstream input_file;
	input_file.open(file_name, std::ios::in);

	input_file >> bit_length;
	logic_expr.bit_length = stoi(bit_length);	//bit_length가 문자열이니까 이걸 정수로

	/*For Debug Only*/
	//std::cout << bit_length << "\n";
	/****************/

	/*파일 읽어서 LogicExpr 구조체 벡터에 넣는 과정*/
	while (!input_file.eof())
	{
		input_file >> temp.flag;
		input_file >> temp.minterm;
		logic_expr.minterm.push_back(temp);
		/*For Debug Only*/
		//std::cout << logic_expr.minterm[logic_expr.minterm_num].flag << "\n" << logic_expr.minterm[logic_expr.minterm_num].minterm << "\n";
		/****************/
		logic_expr.minterm_num++;
	}
	input_file.close();
	return logic_expr;
}

std::vector<PIs> MakeImplicantTable(LogicExpr& logic_expr)
{
	std::vector<PIs> pis;	//Prime Implicant를 저장
	PIs temp_pis; //pis에 넣기 전에 data들을 가공할 변수

	std::vector<std::vector<std::string>> column_table;	//column들이 저장될 table
	std::vector<std::string>* table_ptr;	//for performance;

	std::vector<std::vector<char>> check_table;	//갈매기표시 체크용
	std::vector<char>* check_ptr; //for performance

	std::vector<std::string> column;
	std::string* column_ptr;	//for performance

	std::vector<char> check_column;	//갈매기표시 체크용
	std::vector<std::string> temp_column;	//column들을 가공할 임시 변수

	std::vector<std::string> true_minterm;	//true_minterm을 저장할 변수
	StringData* minterm_ptr = logic_expr.minterm.data(); //for performance

	for (int i = 0; i < logic_expr.minterm_num; i++)
		if (minterm_ptr[i].flag == 'm')	//true minterm을 찾음
			true_minterm.push_back(minterm_ptr[i].minterm);	

	int ham_distance = 0;	//hamming distance가 1차이나는 column에서 차이나는 비트의 index를 저장할 변수
	int bit_length = logic_expr.bit_length;	//bit 길이

	for (int i = 0; i < logic_expr.minterm_num; i++)
		column.emplace_back(logic_expr.minterm[i].minterm);
	int size;	//column 내부의 minterm 개수
	while (1)
	{
		if (column.size() == 0)
			break;
		std::sort(column.begin(), column.end());	//중복원소제거
		column.erase(unique(column.begin(), column.end()), column.end());

		column_ptr = column.data(); //for performance
		size = column.size();
		for (int i = 0; i <= bit_length; i++)
		{
			for (int j = 0; j < size;)
			{
				if (FindHot(column_ptr[j], bit_length) == i)
				{
					temp_column.push_back(column_ptr[j]);
					check_column.push_back(0);
					column.erase(column.begin() + j);	//루프 다돌면 column은 비어있음.
					size--;
				}
				else j++;
			}
			column_table.push_back(temp_column); 
			check_table.push_back(check_column);
			temp_column.clear();
			check_column.clear();
		}
		column.clear();	//혹시 모르니까.
		/*for debug only*/
		/*for (int i = 0; i <= bit_length; i++)
		{
			for (int j = 0; j < column_table[i].size(); j++) std::cout << column_table[i][j] << "\n";
			std::cout << "\n";
		}*/
		size = 0; //다음 생성되는 column의 minterm 개수를 세기 위해 초기화
		check_ptr = check_table.data();		//for performance
		table_ptr = column_table.data();	//for performance
		for (int i = 0; i < bit_length; i++)
		{
			for (int j = 0; j < column_table[i].size(); j++)
			{
				for (int k = 0; k < column_table[i + 1].size(); k++)
				{
					ham_distance = FindHamOne(table_ptr[i][j], table_ptr[i + 1][k], bit_length);
					if (ham_distance != -1)	//Hamming distance가 1이면
					{
						table_ptr[i][j][ham_distance] = '-';	//다른 1자리를 don't care로 만듬
						column.push_back(table_ptr[i][j]);
						check_ptr[i][j] = 1;	//비교해서 Hamming distance가 1이면 갈매기표시.
						check_ptr[i + 1][k] = 1;
						size++;
					}
				}
				//NOTICE : 이렇게하면 table_ptr[bit_length]에 있는 것들이 PI여도 temp_pis에 안들어가니까 마지막에 한번더 처리해줄것.
				if (check_ptr[i][j] == 0)
				{
					temp_pis.PI = table_ptr[i][j];
					for (int i = 0; i < true_minterm.size(); i++)
						if (ComparePIs(temp_pis.PI, true_minterm[i], bit_length))	//True minterm과 Prime Implicant를 비교하여,
							temp_pis.true_minterm.push_back(true_minterm[i]);		//Prime Implicant가 True minterm을 cover한다면 pis에 추가
						pis.push_back(temp_pis);
						temp_pis.true_minterm.clear();
				}
			}
		}
		//NOTICE : 마지막에 처리한 것인데, 최적화 방법 고민해볼것
		for (int i = 0; i < column_table[bit_length].size(); i++)
		{
			if (check_ptr[bit_length][i] == 0)
			{
				temp_pis.PI = table_ptr[bit_length][i];
				for (int j = 0; i < true_minterm.size(); j++)
					if (ComparePIs(temp_pis.PI, true_minterm[j], bit_length))
						temp_pis.true_minterm.push_back(true_minterm[j]);
				pis.push_back(temp_pis);
				temp_pis.true_minterm.clear();
			}
		}
		column_table.clear();
		check_table.clear();
	}
	
	/*for debug only
	//for (int i = 0; i <= logic_expr.bit_length; i++)
	//{
	//	for (int j = 0; j < column_table[i].size(); j++) std::cout << column_table[i][j] << "\n";
	//	std::cout << "\n";
	//}

	for (int i = 0; i < pis.size(); i++)
	{
		std::cout << "PI : " << pis[i].PI << "\n";
		for (int j = 0; j < pis[i].true_minterm.size(); j++)
			std::cout << "TRUE MINTERM : " << pis[i].true_minterm[j] << "\n";
	}
	*/

	return pis;
}

int FindHot(std::string minterm, int bit_length)
{
	int length = 0;
	for (int i = 0; i < bit_length; i++) if (minterm[i] == '1') length++;	//완전탐색을 통해 1의 개수 찾기
	return length;
}

bool ComparePIs(std::string pi, std::string minterm, int bit_length)	//True Minterm이 Prime Implicant로 cover 가능한지 확인
{
	bool is_equal = true;
	for (int i = 0; i < bit_length; i++)
		if ((pi[i] != minterm[i]) && (pi[i] != '-'))	//
			return is_equal = false;
	return is_equal;
}

int FindHamOne(const std::string& str1, const std::string& str2, int bit_length)	//hamming distance가 1이면, 다른 한 bit의 index를 반환하는 함수
{
	int is_one = -1;	//만약 hamming distance != 1이면 -1을 반환한다.
	int dif_num = 0;
	for (int i = 0; i < bit_length; i++)
	{
		if (str1[i] != str2[i])
		{
			dif_num++;
			is_one = i;
		}
	}
	if (dif_num == 1)
		return is_one;	//Hamming distance가 1이면 다른 bit의 index 반환
	else
		return -1;
}