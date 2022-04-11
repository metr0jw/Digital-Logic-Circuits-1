#include <iostream>
#include <vector> 
#include "tabular.h" 
#include "findpi.h"

int main(void)
{
	/********** For Debug Only *************
	std::vector<std::string> logic_expression;
	logic_expression.emplace_back("0122");
	logic_expression.emplace_back("1201");
	logic_expression.emplace_back("1010");
	FindCost(logic_expression, 4); */

	LogicExpr logic_expression = FileRead("input_minterm.txt");
	std::vector<PIs> pis = MakeImplicantTable(logic_expression);

	return 0;
}