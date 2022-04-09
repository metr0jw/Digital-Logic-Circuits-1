#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include "min_bool_equation.h"
#define ull unsigned long long

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
	*/
	const int ROW = logic_expression.size();
	const int COL = true_minterm.size();
	int inner = 0;
	int cnt = 0;
	unsigned long long value;
	bool is_available_pi = false;
	unsigned int pi_idx = 0;
	int remainder = 0;
	int remainder_control = 0;
	
	std::map<int, int> mapped_col;
	std::map<int, int> remapped_col;
	std::vector<int> pi_list;
	std::vector<int>::iterator iter_pi;
	std::vector<int>::iterator iter_temp;
	std::vector<std::string> min_logic_expression;
	std::vector<std::vector<bool>> table(ROW, std::vector<bool>(COL, false));
	std::vector<std::vector<bool>> remove_table(ROW, std::vector<bool>(COL, false));
	std::vector<std::vector<int>> to_remove_row(ROW, std::vector<int>(COL, 0));

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
			std::cout << table[row][mapped_col.at(group[row][inn])] << " ";
		} std::cout << "\n";
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

	// Prime Implicant �� �����ϰ� ���� minterm �ֳ� Ȯ��
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
	if (remainder_control)
	{

	}
	else
	{

	}

	/* FOR DEBUG */
	for (int row = 0; row < ROW; row++) {
		inner = group[row].size();
		for (int col = 0; col < COL; col++) {
			std::cout << table[row][col] << " ";
		} std::cout << "\n";
	}

	return min_logic_expression;
}