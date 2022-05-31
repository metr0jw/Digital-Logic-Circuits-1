#include <iostream>
#include <fstream>
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

	std::ofstream result("result.txt");
	std::vector<std::string> min_bool_expression;
	LogicExpr logic_expression = FileRead("input_minterm.txt");
	std::vector<PIs> pis = MakeImplicantTable(logic_expression);

	StringData* minterm_ptr = logic_expression.minterm.data();
	std::vector<std::string> true_minterm;
	for (int i = 0; i < logic_expression.minterm_num; i++)
		if (minterm_ptr[i].flag == 'm')    //true mintermÀ» Ã£À½
			true_minterm.push_back(minterm_ptr[i].minterm);

	min_bool_expression = CalcMinExpr(true_minterm, pis);

	if (result.is_open()) {
		std::string s;
		if (result.is_open()) {
			for (int i = 0; i < min_bool_expression.size(); i++)
				result << min_bool_expression[i] << "\n";
			result << "\n";

			result << "Cost (# of transistors): " << FindCost(min_bool_expression, logic_expression.bit_length) << "\n";
			result.close();
		}
	} else {
		std::cout << "Failed to write file\n";
	}

	return 0;
}