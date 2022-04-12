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

//std::vector<PIs> MakeImplicantTable(LogicExpr& logic_expr)
//{
//	PIs temp_pis;
//	std::vector<PIs> pis;
//	std::string temp_string;
//	std::vector<std::string> temp_column;
//	std::vector<char> check_column;
//	std::vector<std::string> true_minterm;	//true_minterm�� ������ ����
//	char* check_ptr; //for performance
//	std::vector<std::vector<std::string>> column_table;
//	StringData* minterm_ptr = logic_expr.minterm.data(); //for performance
//	std::string* str_ptr;
//	int cur_minterm_num = 0;	//���� column�� �� ���� minterm�� �ִ��� ���
//
//	std::sort(minterm_ptr, minterm_ptr + logic_expr.minterm_num - 1, CompareMinterm);
//	//For Debug Only//
//	//for (int i = 0; i < logic_expr.minterm_num; i++)
//	//	std::cout << logic_expr.minterm[i].minterm << "\n";
//
//	for (int i = 0; i < logic_expr.minterm_num; i++)
//		if (minterm_ptr[i].flag == 'm')	//true minterm�� ã��
//			true_minterm.push_back(minterm_ptr[i].minterm);
//
//
//	for (int i = 0; i < logic_expr.minterm_num; i++)	//column 1�� �ʱⰪ �߰�
//		temp_column.push_back(minterm_ptr[i].minterm);
//	cur_minterm_num = logic_expr.minterm_num;
//	column_table.push_back(temp_column);	//column table�� column 1 �߰�
//	temp_column.clear();	//temp_column�� ����ϴ� �ʱ�ȭ
//	str_ptr = column_table[0].data();	//for performance
//
//	int cur_pos = 0;	// column ������ minterm�� Ž���Һ���
//	int ham_distance = 0;	//FindHamOne �Լ��� ��ȯ���� ����
//	int cur_column = 0;	//���� column�� ��ġ
//
//	/*For Debug Only*/
//	//std::cout << FindHamOne(str_ptr[1], str_ptr[2], logic_expr.bit_length) << "\n";
//	while (1)
//	{
//		for (int i = 0; i < cur_minterm_num; i++) check_column.push_back(0);
//		check_ptr = check_column.data(); //for performance
//		while (cur_pos < cur_minterm_num)
//		{
//			for (int i = cur_pos + 1; i < cur_minterm_num; i++)
//			{
//				ham_distance = FindHamOne(str_ptr[cur_pos], str_ptr[i], logic_expr.bit_length);	//ham_distance�� 1�̸�, �ٸ� bit�� index ��ȯ
//				if (ham_distance != -1)
//				{
//					temp_string = str_ptr[cur_pos];
//					temp_string[ham_distance] = '2';	//don't care�� �����
//					/*For debug*/
//					//std::cout << temp_string << "\n";
//					//-------
//					temp_column.push_back(temp_string);
//					check_ptr[i] = 1;
//					check_ptr[cur_pos] = 1;
//				}
//			}
//			if (check_ptr[cur_pos] == 0)
//			{
//				temp_pis.PI = str_ptr[cur_pos];
//				for (int i = 0; i < true_minterm.size(); i++)
//					if (ComparePIs(temp_pis.PI, true_minterm[i], logic_expr.bit_length))
//						temp_pis.true_minterm.push_back(true_minterm[i]);
//				pis.push_back(temp_pis);
//				temp_pis.true_minterm.clear();
//			}
//			cur_pos++;
//		}
//
//		if (!temp_column.empty())
//		{
//			std::sort(temp_column.begin(), temp_column.end());	//�ߺ���������
//			temp_column.erase(unique(temp_column.begin(), temp_column.end()), temp_column.end());
//			cur_column++;
//			cur_minterm_num = (int)temp_column.size();
//			column_table.push_back(temp_column);
//			str_ptr = column_table[cur_column].data();	//for performance
//			temp_column.clear(); //�� ������ ���� column ������ ���� �ʱ�ȭ
//			check_column.clear(); //column�� ���ű�ǥ�� ���� �ʱ�ȭ.
//			cur_pos = 0; //���� ��ġ �ʱ�ȭ�Ͽ� ó������ Ž�� �����ϵ���.
//
//			//std::cout << "\n";
//		}
//		else
//			break;
//	}
//
//	/*for debug*/
//	for (int i = 0; i < pis.size(); i++)
//	{
//		std::cout << "PI : " << pis[i].PI << "\n";
//		for (int j = 0; j < pis[i].true_minterm.size(); j++)
//			std::cout << "TRUE MINTERM : " << pis[i].true_minterm[j] << "\n";
//	}
//
//	return pis;
//}

std::vector<PIs> MakeImplicantTable(LogicExpr& logic_expr)
{
	std::vector<PIs> pis;	//Prime Implicant�� ����
	PIs temp_pis; //pis�� �ֱ� ���� data���� ������ ����

	std::vector<std::vector<std::string>> column_table;	//column���� ����� table
	std::vector<std::string>* table_ptr;	//for performance;

	std::vector<std::vector<char>> check_table;	//���ű�ǥ�� üũ��
	std::vector<char>* check_ptr; //for performance

	std::vector<std::string> column;
	std::string* column_ptr;	//for performance

	std::vector<char> check_column;	//���ű�ǥ�� üũ��
	std::vector<std::string> temp_column;	//column���� ������ �ӽ� ����

	std::vector<std::string> true_minterm;	//true_minterm�� ������ ����
	StringData* minterm_ptr = logic_expr.minterm.data(); //for performance

	for (int i = 0; i < logic_expr.minterm_num; i++)
		if (minterm_ptr[i].flag == 'm')	//true minterm�� ã��
			true_minterm.push_back(minterm_ptr[i].minterm);	

	int ham_distance = 0;	//hamming distance�� 1���̳��� column���� ���̳��� ��Ʈ�� index�� ������ ����
	int bit_length = logic_expr.bit_length;	//bit ����

	for (int i = 0; i < logic_expr.minterm_num; i++)
		column.emplace_back(logic_expr.minterm[i].minterm);
	int size;	//column ������ minterm ����
	while (1)
	{
		if (column.size() == 0)
			break;
		std::sort(column.begin(), column.end());	//�ߺ���������
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
					column.erase(column.begin() + j);	//���� �ٵ��� column�� �������.
					size--;
				}
				else j++;
			}
			column_table.push_back(temp_column); 
			check_table.push_back(check_column);
			temp_column.clear();
			check_column.clear();
		}
		column.clear();	//Ȥ�� �𸣴ϱ�. �ٵ� �Ƹ� �Ⱥ�������� ���� ����
		/*for debug only*/
		/*for (int i = 0; i <= bit_length; i++)
		{
			for (int j = 0; j < column_table[i].size(); j++) std::cout << column_table[i][j] << "\n";
			std::cout << "\n";
		}*/
		size = 0; //���� �����Ǵ� column�� minterm ������ ���� ���� �ʱ�ȭ
		check_ptr = check_table.data();		//for performance
		table_ptr = column_table.data();	//for performance
		for (int i = 0; i < bit_length; i++)
		{
			for (int j = 0; j < column_table[i].size(); j++)
			{
				for (int k = 0; k < column_table[i + 1].size(); k++)
				{
					ham_distance = FindHamOne(table_ptr[i][j], table_ptr[i + 1][k], bit_length);
					if (ham_distance != -1)
					{
						table_ptr[i][j][ham_distance] = '2';	//�ٸ� 1�ڸ��� don't care�� ����
						column.push_back(table_ptr[i][j]);
						check_ptr[i][j] = 1;
						check_ptr[i + 1][k] = 1;
						size++;
					}
				}
				//NOTICE : �̷����ϸ� table_ptr[bit_length]�� �ִ� �͵��� PI���� temp_pis�� �ȵ��ϱ� �������� �ѹ��� ó�����ٰ�.
				if (check_ptr[i][j] == 0)
				{
					temp_pis.PI = table_ptr[i][j];
					for (int i = 0; i < true_minterm.size(); i++)
						if (ComparePIs(temp_pis.PI, true_minterm[i], bit_length))
							temp_pis.true_minterm.push_back(true_minterm[i]);
						pis.push_back(temp_pis);
						temp_pis.true_minterm.clear();
				}
			}
		}
		//NOTICE : �������� ó���� ���ε�, ����ȭ ��� ����غ���
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
	
	/*for debug only*/
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

	return pis;
}

int FindHot(std::string minterm, int bit_length)
{
	int length = 0;
	for (int i = 0; i < bit_length; i++) if (minterm[i] == '1') length++;
	return length;
}

bool ComparePIs(std::string pi, std::string minterm, int bit_length)
{
	bool is_equal = true;
	for (int i = 0; i < bit_length; i++)
		if ((pi[i] != minterm[i]) && (pi[i] != '2'))
			return is_equal = false;
	return is_equal;
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