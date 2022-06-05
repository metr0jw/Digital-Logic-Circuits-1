#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <bitset>
#include "min_bool_equation.h"
#include "tabular.h"

using ull = unsigned long long;

std::vector<std::string> CalcMinExpr(const std::vector<std::string>& true_minterm, const std::vector<PIs>& logic_expr) {
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
	* COL : true minterm�� ����
	* ROW : logic expression�� ũ��
	* is_available_pi : Prime Implicant �����ϴ��� ���� �Ǵ�, �� ���� �̿��� �б� ����
	* pi_idx : Prime Implicant�� �����ϴ� row ����
	* mapped_col : �޸� �����ϱ� ���� group ��ȣ�� ����
	* min_logic_expression : minimum logic expression
	* table : true minterm�� logic expression�� ǥ���� vector(true for x, false for none)
	* iter : vector�� iterator�� ����ϱ� ���� ����
	* remainder : PI�� ����� ���� minterm�� ����
	* remainder_control : remainder�� ��(minimum value)
	* petrick : Petrick's Method���� ��� P�� ��
	* petrick_bool_equation : Petrick's method���� boolean equation�� ����ϱ� ���� string�� vector
	* is_added : Petrick's method���� �ش� ���� ���� ���� �߰��Ǿ����� �Ǻ�
	* to_opt : Petrick's method���� ����ȭ��Ű�� �ϴ� P�� sum of products
	*/
	std::vector<std::vector<ull>> group;
	const ull COL = true_minterm.size();
	const ull ROW = logic_expr.size();
	std::vector<std::vector<bool>> table(ROW, std::vector<bool>(COL, false));

	bool is_available_pi = false;
	bool is_added = false;
	ull pi_idx = 0;
	ull remainder = 0;
	ull remainder_control = 0;

	std::map<std::string, ull> mapped_col;
	std::vector<ull> pi_list;
	std::vector<ull>::iterator iter_pi;
	std::vector<ull>::iterator iter_temp;
	std::vector<std::string> min_logic_expression;
	std::vector<std::vector<std::string>> petrick;
	std::vector<std::string> petrick_bool_equation;
	std::vector<std::string> petrick_temp;
	std::vector<bool> to_opt;

	std::vector<std::string> logic_expression;

	for (ull col = 0; col < COL; col++)
	{
		mapped_col[true_minterm[col]] = col;
	}

	for (int idx = 0; idx < logic_expr.size(); idx++)
	{
		std::vector<ull> temp;
		temp.clear();
		logic_expression.push_back(logic_expr[idx].PI);
		for (ull group_size = 0; group_size < logic_expr[idx].true_minterm.size(); group_size++)
		{
			temp.push_back(mapped_col[logic_expr[idx].true_minterm[group_size]]);
		}
		group.push_back(temp);
	}
	mapped_col.clear();
	
	// true minterm, logic expression ǥ���� ���̺� ����
	for (ull row = 0, inner = 0; row < ROW; row++)
	{
		inner = group[row].size();
		for (ull inn = 0; inn < inner; inn++)
		{
			table[row][group[row][inn]] = true;
		}
	}

	// Essential Prime Implicant ã��
	for (ull col = 0, cnt = 0; col < COL; col++, cnt = 0)
	{
		for (ull row = 0; row < ROW; row++)
		{
			if (table[row][col])
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
					table[row][col_rm] = false;
				}
			}
		}
	}
	pi_list.clear();

	// Prime Implicant �� �����ϰ� ���� minterm �ִ��� Ȯ��
	for (ull col = 0, cnt = 0; col < COL; col++, cnt = 0)
	{
		remainder = 0;
		for (ull row = 0; row < ROW; row++)
		{
			if (table[row][col])
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
	if (remainder)
	{
		for (ull col = 0; col < COL; col++)
		{
			is_added = false;
			petrick_temp = {};
			for (ull row = 0; row < ROW; row++)
			{
				if (table[row][col])
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
			for (ull col = 0; col < petrick[0].size(); col++)
			{
				for (ull inner = 0; inner < petrick[1].size(); inner++)
				{
					petrick_temp.push_back(petrick[0][col] + petrick[1][inner]);
					is_added = true;
				}
			}
			if (is_added)
			{
				petrick[0] = petrick_temp;
			} petrick.erase(petrick.begin() + 1);
		} while (petrick.size() > 1);

		petrick_temp.clear();
		petrick_bool_equation = petrick[0];
		petrick.clear();

		for (ull row = 0; row < petrick_bool_equation.size(); row++)
		{
			std::sort(petrick_bool_equation[row].begin(), petrick_bool_equation[row].end());
			petrick_bool_equation[row].erase(std::unique(petrick_bool_equation[row].begin(), petrick_bool_equation[row].end()), petrick_bool_equation[row].end());
		}

		ull min = ULLONG_MAX;
		ull min_idx = 0;
		for (ull idx = 0; idx < petrick_bool_equation.size(); idx++)
		{
			if (petrick_bool_equation[idx].size() < min)
			{
				min = petrick_bool_equation[idx].size();
				min_idx = idx;
			}
		}

		for (ull idx = 0; idx < petrick_bool_equation[min_idx].size(); idx++)
		{
			min_logic_expression.push_back(logic_expression[petrick_bool_equation[min_idx][idx] - '0']);
		} petrick_bool_equation.clear();
	}

	return min_logic_expression;
}