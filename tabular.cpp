#include "tabular.h"
#include <iostream>
#include <vector>
#include <fstream>

LogicExpr FileRead(const char file_name[])
{
	std::string bit_length;	
	StringData temp;	//���⿡ �����͸� �����ؼ� ��ȯ�� ����
	
	LogicExpr logic_expr;
	logic_expr.minterm_num = 0;
	std::ifstream input_file;
	input_file.open(file_name, std::ios::in);

	input_file >> bit_length;
	logic_expr.bit_length = stoi(bit_length);

	/*For Debug Only*/
	//std::cout << bit_length << "\n";
	/****************/

	/*���� �о LogicExpr ����ü ���Ϳ� �ִ� ����*/
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

