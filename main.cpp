#include <iostream>
#include <vector> 
#include "tabular.h" 
#include "findpi.h"
#include "min_bool_equation.h"

int main(void)
{	/********** For Debug Only *************
	std::vector<std::string> logic_expression;
	std::vector<std::string> true_minterm;
	std::vector<std::vector<int>> group
	{
		{{1}, {5}},
		{{5}, {7}},
		{{6}, {7}},
		{{0}, {1}, {8}, {9}},
		{{0}, {2}, {8}, {10}},
		{{2}, {6}, {10}, {14}}
	};
	int bit_length = 4;

	logic_expression.emplace_back("0201");
	logic_expression.emplace_back("0121");
	logic_expression.emplace_back("0112");
	logic_expression.emplace_back("2002");
	logic_expression.emplace_back("2020");
	logic_expression.emplace_back("2210");

	true_minterm.emplace_back("0000");
	true_minterm.emplace_back("0001");
	true_minterm.emplace_back("0010");
	true_minterm.emplace_back("0101");
	true_minterm.emplace_back("0110");
	true_minterm.emplace_back("0111");
	true_minterm.emplace_back("1000");
	true_minterm.emplace_back("1001");
	true_minterm.emplace_back("1010");
	true_minterm.emplace_back("1110");

	CalcMinExpr(true_minterm, logic_expression, group, 4);
	*/

	return 0;
}