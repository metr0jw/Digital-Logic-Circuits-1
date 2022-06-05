#include <iostream>
#include <vector> 
#include "tabular.h" 
#include "findpi.h"
#include "min_bool_equation.h"

int main(void)
{
	/********** For Debug Only *************
	std::vector<std::string> logic_expression;
	logic_expression.emplace_back("0122");
	logic_expression.emplace_back("1201");
	logic_expression.emplace_back("1010");
	FindCost(logic_expression, 4); */

	std::vector<std::string> min_bool_expression;
	LogicExpr logic_expression = FileRead("input_minterm.txt");
	std::vector<PIs> pis = MakeImplicantTable(logic_expression);

	StringData* minterm_ptr = logic_expression.minterm.data();
	std::vector<std::string> true_minterm;
	for (int i = 0; i < logic_expression.minterm_num; i++)
		if (minterm_ptr[i].flag == 'm')    //true mintermÀ» Ã£À½
			true_minterm.push_back(minterm_ptr[i].minterm);

	min_bool_expression = CalcMinExpr(true_minterm, pis);
	for (int i = 0; i<min_bool_expression.size(); i++)
		std::cout << min_bool_expression[i] << "\n";
	std::cout << "\n";

	std::cout << "Cost (# of transistors): " << FindCost(min_bool_expression, logic_expression.bit_length) << "\n";

	return 0;
}