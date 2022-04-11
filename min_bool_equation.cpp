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
	* petrick : Petrick's Method에서 모든 P의 곱
	* petrick_bool_equation : Petrick's method에서 boolean equation을 계산하기 위한 string의 vector
	* mapped_row : Petrick's Method에서 사용하는 mapped_row
	* is_added : Petrick's method에서 해당 열에 남은 항이 추가되었는지 판별
	* to_opt : Petrick's method에서 최적화시키려 하는 P의 sum of products
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
		}
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

	// Prime Implicant가 존재한다면 PI의 열 및 행을 table에서 각각 제거함
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

	// Prime Implicant 열 제거하고 남는 minterm 있는지 확인
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