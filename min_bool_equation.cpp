#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include "min_bool_equation.h"

std::vector<std::string> CalcMinExpr(const std::vector<std::string>& true_minterm, const std::vector<std::string>& logic_expression, const std::vector<std::vector<int>>& group, const int bit_length) {
	/*
	* true_minterm : {"0000", "0100", "1101", ..., "1111"}
	*****************************************************************
	* Logic Expression : Don't Care = 2 ex) not appeared in minterm *
	*					     FALSE = 0 ex) A`					    *
	*					      TRUE = 1 ex) A					    *
	*****************************************************************
	* group : logic_expression의 크기와 같음, {[[0], [1]], [[5], [7]], [[6], [7]], [[0], [1], [8], [9]]}
	* number of column : true minterm 개수
	* number of row : logic expression 항 개수
	*/
	int row = logic_expression.size();
	int col = true_minterm.size();
	int inner = 0;
	unsigned long long value;
	bool is_available_pi = false;
	std::map<int, int> mapped_col;
	std::string min_logic_expression = {};
	std::vector<std::vector<bool>> table(row, std::vector<bool>(col, false));

	for (int i = 0; i < col; i++)
	{
		value = std::stol(true_minterm[i], 0, 2);
		mapped_col[value] = i;
	}

	for (int i = 0, start = 0; i < row; i++)
	{
		inner = group[i].size();
		for (int j = 0; j < col; j++)
		{
			for (int k = 0; k < inner; k++)
			{
				table[i][mapped_col.at(group[i][k])] = true;
			}
			std::cout << table[i][j] << " ";
		} std::cout << std::endl;
	}

	return logic_expression;
}