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
		temp.num_of_hot = 0;	//minterm 내부의 1 개수 저장할 변수
		temp.num_of_hot = FindHot(temp.minterm, logic_expr.bit_length);
		//std::cout << temp.num_of_hot << "\n";
		logic_expr.minterm.push_back(temp);
		/*For Debug Only*/
		//std::cout << logic_expr.minterm[logic_expr.minterm_num].flag << "\n" << logic_expr.minterm[logic_expr.minterm_num].minterm << "\n";
		/****************/
		logic_expr.minterm_num++;
	}
	input_file.close();
	return logic_expr;
}


//TODO : True Minterm, PIs 반환 동시에
std::vector<std::string> MakeImplicantTable(LogicExpr& logic_expr)
{
	std::vector<std::string> PIs;	//it will store stared elements.
	std::string temp_string;
	std::vector<std::string> temp_column;
	std::vector<char> check_column;
	char* check_ptr; //for performance
	std::vector<std::vector<std::string>> column_table;
	StringData* minterm_ptr = logic_expr.minterm.data(); //for performance
	std::string* str_ptr;
	int cur_minterm_num = 0;	//현재 column에 몇 개의 minterm이 있는지 기록

	std::sort(minterm_ptr, minterm_ptr + logic_expr.bit_length - 1, CompareMinterm);
	//For Debug Only//
	//for (int i = 0; i < logic_expr.minterm_num; i++)
	//	std::cout << logic_expr.minterm[i].minterm << "\n";

	for (int i = 0; i < logic_expr.minterm_num; i++)	//column 1에 초기값 추가
		temp_column.push_back(minterm_ptr[i].minterm); 
	cur_minterm_num = logic_expr.minterm_num;
	column_table.push_back(temp_column);	//column table에 column 1 추가
	temp_column.clear();	//temp_column을 써야하니 초기화
	str_ptr = column_table[0].data();	//for performance

	int cur_pos = 0;	// column 내부의 minterm을 탐색할변수
	int ham_distance = 0;	//FindHamOne 함수의 반환값을 저장
	int cur_column = 0;	//현재 column의 위치
	//int temp_num = 0; //임시로 minterm 개수 저장할것.
	
	/*For Debug Only*/
	//std::cout << FindHamOne(str_ptr[1], str_ptr[2], logic_expr.bit_length) << "\n";
	while (1)
	{
		for (int i = 0; i < cur_minterm_num; i++) check_column.push_back(0);
		check_ptr = check_column.data(); //for performance
		while (cur_pos < cur_minterm_num)
		{
			for (int i = cur_pos + 1; i < cur_minterm_num; i++)
			{
				ham_distance = FindHamOne(str_ptr[cur_pos], str_ptr[i], logic_expr.bit_length);	//ham_distance가 1이면, 다른 bit의 index 반환
				if (ham_distance != -1)
				{
					temp_string = str_ptr[cur_pos];
					temp_string[ham_distance] = '2';	//don't care로 만든다
					/*For debug*/
					//std::cout << temp_string << "\n";
					//-------
					temp_column.push_back(temp_string);
					check_ptr[i] = 1;
					check_ptr[cur_pos] = 1;
				}
			}
			if (check_ptr[cur_pos] == 0)
				PIs.push_back(str_ptr[cur_pos]);
			cur_pos++;
		}
		
		if (!temp_column.empty())
		{
			std::sort(temp_column.begin(), temp_column.end());	//중복원소제거
			temp_column.erase(unique(temp_column.begin(), temp_column.end()), temp_column.end());
			cur_column++;
			cur_minterm_num = (int)temp_column.size();
			column_table.push_back(temp_column);
			str_ptr = column_table[cur_column].data();	//for performance
			temp_column.clear(); //다 썼으니 다음 column 저장을 위해 초기화
			check_column.clear(); //column의 갈매기표를 전부 초기화.
			cur_pos = 0; //현재 위치 초기화하여 처음부터 탐색 가능하도록.

			//std::cout << "\n";
		}
		else
			break;
	}

	/*for debug*/
	/*for (int i = 0; i < PIs.size(); i++)
		std::cout << PIs[i] << "\n";*/

	return PIs;
}

int FindHot(std::string minterm, int bit_length)	//문자열 내의 1 개수를 찾는 함수
{
	int num_of_hot = 0;
	for (int i = 0; i < bit_length; i++) if (minterm[i] == '1') num_of_hot++;
	return num_of_hot;
}

bool CompareMinterm(const StringData& data1, const StringData& data2) //sort를 위한 비교함수.
{	//StringData 구조체는 내부에 string만 담겨있는게 아니므로, StringData 내부의 minterm을 비교하여 StringData를 sort하기 위함이다.
	if (data1.minterm.compare(data2.minterm) < 0) return true;
	else return false;
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
		return is_one;
	else
		return -1;
}