#include "tabular.h"
#include <iostream>
#include <vector>
#include <fstream>

LogicExpr FileRead(const char file_name[])
{
	std::string bit_length;	
	StringData temp;	//여기에 데이터를 저장해서 반환할 것임
	
	LogicExpr logic_expr;
	logic_expr.minterm_num = 0;
	logic_expr.bit_length = '0';
	std::ifstream input_file;
	input_file.open(file_name, std::ios::in);

	input_file >> bit_length;
	logic_expr.bit_length = stoi(bit_length);

	/*For Debug Only*/
	//std::cout << bit_length << "\n";
	/****************/

	/*파일 읽어서 LogicExpr 구조체 벡터에 넣는 과정*/
	while (!input_file.eof())	
	{
		input_file >> temp.flag;
		input_file >> temp.minterm;
		logic_expr.minterm.emplace_back(temp);
		/*For Debug Only*/
		//std::cout << logic_expr.minterm[logic_expr.minterm_num].flag << "\n" << logic_expr.minterm[logic_expr.minterm_num].minterm << "\n";
		/****************/
		logic_expr.minterm_num++;
	}
	input_file.close();
	return logic_expr;
}

