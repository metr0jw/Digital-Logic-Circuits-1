#include "tabular.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

LogicExpr FileRead(const char file_name[])
{
	std::string bit_length;
	StringData temp;	//���⿡ �����͸� �����ؼ� ��ȯ�� ����

	LogicExpr logic_expr;
	logic_expr.minterm_num = 0;
	logic_expr.bit_length = '0';
	std::ifstream input_file;
	input_file.open(file_name, std::ios::in);

	input_file >> bit_length;
	logic_expr.bit_length = stoi(bit_length);	//bit_length�� ���ڿ��̴ϱ� �̰� ������

	/*For Debug Only*/
	//std::cout << bit_length << "\n";
	/****************/

	/*���� �о LogicExpr ����ü ���Ϳ� �ִ� ����*/
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


//TODO : True Minterm, PIs ��ȯ ���ÿ�
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
	int cur_minterm_num = 0;	//���� column�� �� ���� minterm�� �ִ��� ���

	std::sort(minterm_ptr, minterm_ptr + logic_expr.minterm_num - 1, CompareMinterm);
	//For Debug Only//
	//for (int i = 0; i < logic_expr.minterm_num; i++)
	//	std::cout << logic_expr.minterm[i].minterm << "\n";

	for (int i = 0; i < logic_expr.minterm_num; i++)	//column 1�� �ʱⰪ �߰�
		temp_column.push_back(minterm_ptr[i].minterm); 
	cur_minterm_num = logic_expr.minterm_num;
	column_table.push_back(temp_column);	//column table�� column 1 �߰�
	temp_column.clear();	//temp_column�� ����ϴ� �ʱ�ȭ
	str_ptr = column_table[0].data();	//for performance

	int cur_pos = 0;	// column ������ minterm�� Ž���Һ���
	int ham_distance = 0;	//FindHamOne �Լ��� ��ȯ���� ����
	int cur_column = 0;	//���� column�� ��ġ
	//int temp_num = 0; //�ӽ÷� minterm ���� �����Ұ�.
	
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
				ham_distance = FindHamOne(str_ptr[cur_pos], str_ptr[i], logic_expr.bit_length);	//ham_distance�� 1�̸�, �ٸ� bit�� index ��ȯ
				if (ham_distance != -1)
				{
					temp_string = str_ptr[cur_pos];
					temp_string[ham_distance] = '2';	//don't care�� �����
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
			std::sort(temp_column.begin(), temp_column.end());	//�ߺ���������
			temp_column.erase(unique(temp_column.begin(), temp_column.end()), temp_column.end());
			cur_column++;
			cur_minterm_num = (int)temp_column.size();
			column_table.push_back(temp_column);
			str_ptr = column_table[cur_column].data();	//for performance
			temp_column.clear(); //�� ������ ���� column ������ ���� �ʱ�ȭ
			check_column.clear(); //column�� ���ű�ǥ�� ���� �ʱ�ȭ.
			cur_pos = 0; //���� ��ġ �ʱ�ȭ�Ͽ� ó������ Ž�� �����ϵ���.

			//std::cout << "\n";
		}
		else
			break;
	}

	/*for debug*/
	for (int i = 0; i < PIs.size(); i++)
		std::cout << PIs[i] << "\n";

	return PIs;
}

bool CompareMinterm(const StringData& data1, const StringData& data2) //sort�� ���� ���Լ�.
{	//StringData ����ü�� ���ο� string�� ����ִ°� �ƴϹǷ�, StringData ������ minterm�� ���Ͽ� StringData�� sort�ϱ� �����̴�.
	if (data1.minterm.compare(data2.minterm) < 0) return true;
	else return false;
}

int FindHamOne(const std::string& str1, const std::string& str2, int bit_length)	//hamming distance�� 1�̸�, �ٸ� �� bit�� index�� ��ȯ�ϴ� �Լ�
{																					
	int is_one = -1;	//���� hamming distance != 1�̸� -1�� ��ȯ�Ѵ�.
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