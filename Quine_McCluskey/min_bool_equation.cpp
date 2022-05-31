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
	* group : logic_expression의 크기와 같음, {[[0], [1]], [[5], [7]], [[6], [7]], [[0], [1], [8], [9]]}
	* number of column : true minterm 개수
	* number of row : logic expression 항 개수
	*
	* COL : true minterm의 개수
	* ROW : logic expression의 크기
	* is_available_pi : Prime Implicant 존재하는지 여부 판단, 이 변수 이용해 분기 제작
	* pi_idx : Prime Implicant가 존재하는 row 변수
	* mapped_col : 메모리 절약하기 위해 group 번호를 매핑
	* min_logic_expression : minimum logic expression
	* table : true minterm과 logic expression을 표현한 vector(true for x, false for none)
	* iter : vector를 iterator로 사용하기 위해 선언
	* remainder : PI를 지우고 남은 minterm의 개수
	* remainder_control : remainder와 비교(minimum value)
	* petrick : Petrick's Method에서 모든 P의 곱
	* petrick_bool_equation : Petrick's method에서 boolean equation을 계산하기 위한 string의 vector
	* is_added : Petrick's method에서 해당 열에 남은 항이 추가되었는지 판별
	* to_opt : Petrick's method에서 최적화시키려 하는 P의 sum of products
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

	// 메모리 절약 위해 hash 테이블 이용해 column 축소
	for (ull col = 0; col < COL; col++)
	{
		mapped_col[true_minterm[col]] = col;
	}

	// group이라는 변수에 prime implicant 만드는데 어떤 minterm 사용되었는지 매핑
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

	// group의 중복 제거
	for (int idx = 0; idx < group.size(); idx++)
	{
		sort(group[idx].begin(), group[idx].end());
		group[idx].erase(std::unique(group[idx].begin(), group[idx].end()), group[idx].end());

	}
	
	// true minterm, logic expression 표현한 테이블 제작
	for (ull row = 0, inner = 0; row < ROW; row++)
	{
		inner = group[row].size();
		for (ull inn = 0; inn < inner; inn++)
		{
			table[row][group[row][inn]] = true;
		}
	}

	// Essential Prime Implicant 찾기
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
					table[row][col_rm] = false;
				}
			}
		}
	}
	pi_list.clear();

	// Prime Implicant 열 제거하고 남는 minterm 있는지 확인
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

		// 분배법칙 과정에서 중복된 minterm 제거
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