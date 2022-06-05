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
		column.clear();	//Ȥ�� �𸣴ϱ�.
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
					if (ham_distance != -1)	//Hamming distance�� 1�̸�
					{
						table_ptr[i][j][ham_distance] = '-';	//�ٸ� 1�ڸ��� don't care�� ����
						column.push_back(table_ptr[i][j]);
						check_ptr[i][j] = 1;	//���ؼ� Hamming distance�� 1�̸� ���ű�ǥ��.
						check_ptr[i + 1][k] = 1;
						size++;
					}
				}
				//NOTICE : �̷����ϸ� table_ptr[bit_length]�� �ִ� �͵��� PI���� temp_pis�� �ȵ��ϱ� �������� �ѹ��� ó�����ٰ�.
				if (check_ptr[i][j] == 0)
				{
					temp_pis.PI = table_ptr[i][j];
					for (int i = 0; i < true_minterm.size(); i++)
						if (ComparePIs(temp_pis.PI, true_minterm[i], bit_length))	//True minterm�� Prime Implicant�� ���Ͽ�,
							temp_pis.true_minterm.push_back(true_minterm[i]);		//Prime Implicant�� True minterm�� cover�Ѵٸ� pis�� �߰�
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
	for (int i = 0; i < bit_length; i++) if (minterm[i] == '1') length++;	//����Ž���� ���� 1�� ���� ã��
	return length;
}

bool ComparePIs(std::string pi, std::string minterm, int bit_length)	//True Minterm�� Prime Implicant�� cover �������� Ȯ��
{
	bool is_equal = true;
	for (int i = 0; i < bit_length; i++)
		if ((pi[i] != minterm[i]) && (pi[i] != '-'))	//
			return is_equal = false;
	return is_equal;
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
		return is_one;	//Hamming distance�� 1�̸� �ٸ� bit�� index ��ȯ
	else
		return -1;
}