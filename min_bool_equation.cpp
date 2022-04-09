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
	* group : logic_expression의 크기와 같음, {[[0], [1]], [[5], [7]], [[6], [7]], [[0], [1], [8], [9]]}
	* number of column : true minterm 개수
	* number of row : logic expression 항 개수
	* 
	* row : logic expression의 크기
	* col : true minterm의 개수
	* inner : table 만드는 데 필요한 내부 loop 변수
	* is_available_pi : Prime Implicant 존재하는지 여부 판단, 이 변수 이용해 분기 제작
	* pi_idx : Prime Implicant가 존재하는 row 변수
	* mapped_col : 메모리 절약하기 위해 group 번호를 매핑
	* remapped_col : mapped_col 번호를 원래대로 리매핑
	* _group : group이 vector로 된 것을 set으로 중복 제거, 리스트 방식으로 접근
	* min_logic_expression : minimum logic expression
	* table : true minterm과 logic expression을 표현한 vector(true for x, false for none)
	* remove_table : 필요 없는 항을 제거한 table
	* remove_col : prime implicant 구하는 과정에서 해당 column이 지워졌는지 판단
	* iter : vector를 iterator로 사용하기 위해 선언
	* remainder : PI를 지우고 남은 minterm의 개수
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

	// true minterm, logic expression 표현한 테이블 제작
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

	// Essential Prime Implicant 찾기
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

	// Prime Implicant 열 제거하고 남는 minterm 있나 확인
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