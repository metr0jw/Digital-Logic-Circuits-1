#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include "min_bool_equation.h"
#define ull unsigned long long
#define uint unsigned int


std::vector<std::string> CalcMinExpr(const std::vector<std::string>& true_minterm, const std::vector<std::string>& logic_expression, const std::vector<std::vector<int>>& group, const int bit_length) {
	/*
	* true_minterm : {"0000", "0100", "1101", ..., "1111"}
	*****************************************************************
	* Logic Expression : Don't Care = 2 ex) not appeared in minterm *
	*					     FALSE = 0 ex) A`					    *
	*					      TRUE = 1 ex) A					    *
	*****************************************************************
	* group : logic_expression�� ũ��� ����, {[[0], [1]], [[5], [7]], [[6], [7]], [[0], [1], [8], [9]]}
	* number of column : true minterm ����
	* number of row : logic expression �� ����
	* 
	* row : logic expression�� ũ��
	* col : true minterm�� ����
	* inner : table ����� �� �ʿ��� ���� loop ����
	* is_available_pi : Prime Implicant �����ϴ��� ���� �Ǵ�, �� ���� �̿��� �б� ����
	* pi_idx : Prime Implicant�� �����ϴ� row ����
	* mapped_col : �޸� �����ϱ� ���� group ��ȣ�� ����
	* remapped_col : mapped_col ��ȣ�� ������� ������
	* _group : group�� vector�� �� ���� set���� �ߺ� ����, ����Ʈ ������� ����
	* min_logic_expression : minimum logic expression
	* table : true minterm�� logic expression�� ǥ���� vector(true for x, false for none)
	* remove_table : �ʿ� ���� ���� ������ table
	* remove_col : prime implicant ���ϴ� �������� �ش� column�� ���������� �Ǵ�
	* iter : vector�� iterator�� ����ϱ� ���� ����
	* remainder : PI�� ����� ���� minterm�� ����
	* petrick : Petrick's Method���� ��� P�� ��
	* petrick_bool_equation : Petrick's method���� boolean equation�� ����ϱ� ���� string�� vector
	* mapped_row : Petrick's Method���� ����ϴ� mapped_row
	* is_added : Petrick's method���� �ش� ���� ���� ���� �߰��Ǿ����� �Ǻ�
	* to_opt : Petrick's method���� ����ȭ��Ű�� �ϴ� P�� sum of products
	*/
	const int ROW = logic_expression.size();
	const int COL = true_minterm.size();
	int inner = 0;
	int cnt = 0;
	unsigned long long value;
	bool is_available_pi = false;
	bool is_added = false;
	unsigned int pi_idx = 0;
	int remainder = 0;
	int remainder_control = 0;
	
	std::map<int, int> mapped_col;
	std::map<int, int> remapped_col;
	std::map<int, int> mapped_row;
	std::vector<int> pi_list;
	std::vector<int>::iterator iter_pi;
	std::vector<int>::iterator iter_temp;
	std::vector<std::string> min_logic_expression;
	std::vector<std::vector<bool>> table(ROW, std::vector<bool>(COL, false));
	std::vector<std::vector<bool>> remove_table(ROW, std::vector<bool>(COL, false));
	std::vector<std::vector<std::string>> petrick;
	std::vector<std::string> petrick_bool_equation;
	std::vector<std::string> petrick_temp;
	std::vector<bool> to_opt;

	// true minterm, logic expression ǥ���� ���̺� ����
	for (int col = 0; col < COL; col++)
	{
		value = std::stol(true_minterm[col], 0, 2);
		mapped_col[value] = col;
		remapped_col[col] = value;
	}

	for (int row = 0; row < ROW; row++)
	{
		inner = group[row].size();
		for (int inn = 0; inn < inner; inn++)
		{
			table[row][mapped_col.at(group[row][inn])] = true;
		}
	}

	// Essential Prime Implicant ã��
	for (int col = 0; col< COL; col++, cnt = 0)
	{
		for (int row = 0; row < ROW; row++)
		{
			if (table[row][col] == true)
			{
				cnt++;
				pi_idx = row;
			}
		}
		
		if (cnt == 1)
		{
			pi_list.push_back(pi_idx);
			is_available_pi = true;
			min_logic_expression.push_back(logic_expression[pi_idx]);
		}
	}

	// Prime Implicant�� �����Ѵٸ� PI�� �� �� ���� table���� ���� ������
	if (is_available_pi)
	{
		for (iter_pi = pi_list.begin(); iter_pi != pi_list.end(); iter_pi++)
		{
			for (int col_rm = group[*iter_pi][0], idx_temp = 0; idx_temp < group[*iter_pi].size(); idx_temp++)
			{
				col_rm = group[*iter_pi][idx_temp];
				for (int row = 0; row < ROW; row++)
				{
					table[row][mapped_col.at(col_rm)] = false;
				}
			}
		}
	}

	// Prime Implicant �� �����ϰ� ���� minterm �ִ��� Ȯ��
	for (int col = 0; col < COL; col++, cnt = 0)
	{
		remainder = 0;
		for (int row = 0; row < ROW; row++)
		{
			if (table[row][col] == true)
			{
				remainder++;
			}
		}
		if (remainder_control < remainder)
		{
			remainder_control = remainder;
		}
	}

	// Petrick's method
	for (int col = 0; col < COL; col++)
	{
		is_added = false;
		petrick_temp = {};
		for (int row = 0; row < ROW; row++)
		{
			if (table[row][col] == true)
			{
				petrick_temp.push_back(std::to_string(row));
				is_added = true;
				sort(petrick_temp.begin(), petrick_temp.end());
				petrick_temp.erase(unique(petrick_temp.begin(), petrick_temp.end()), petrick_temp.end());
			}
		}
		if (is_added)
		{
			petrick.push_back(petrick_temp);
		}
	}

	do
	{
		is_added = false;
		petrick_temp.clear();
		for (int col = 0; col < petrick[0].size(); col++)
		{
			for (int inner = 0; inner < petrick[1].size(); inner++)
			{
				petrick_temp.push_back(petrick[0][col] + petrick[1][inner]);
				is_added = true;
			}
		}
		if (is_added)
		{
			petrick[0] = petrick_temp;
		} petrick.erase(petrick.begin()+1);
	} while (petrick.size() > 1);

	petrick_temp.clear();
	petrick_bool_equation = petrick[0];
	petrick.clear();

	for (int row = 0; row < petrick_bool_equation.size(); row++)
	{
		std::sort(petrick_bool_equation[row].begin(), petrick_bool_equation[row].end());
		petrick_bool_equation[row].erase(std::unique(petrick_bool_equation[row].begin(), petrick_bool_equation[row].end()), petrick_bool_equation[row].end());
	}

	int min = 100000;
	int min_idx = 0;
	for (int idx = 0; idx < petrick_bool_equation.size(); idx++)
	{
		if (petrick_bool_equation[idx].size() < min)
		{
			min = petrick_bool_equation[idx].size();
			min_idx = idx;
		}
	}

	for (int idx = 0; idx < petrick_bool_equation[min_idx].size(); idx++)
	{
		min_logic_expression.push_back(logic_expression[petrick_bool_equation[min_idx][idx] - '0']);
	}

	/* FOR DEBUG */
	for (int row = 0; row < ROW; row++) {
		inner = group[row].size();
		for (int col = 0; col < COL; col++) {
			std::cout << table[row][col] << " ";
		} std::cout << "\n";
	}
	for (int size = 0; size < min_logic_expression.size(); size++)
	{
		std::cout << min_logic_expression[size] << " ";
	} std::cout << "\n";

	return min_logic_expression;
}