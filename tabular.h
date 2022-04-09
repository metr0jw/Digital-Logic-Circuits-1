#pragma once
#include <iostream>
#include <string>
#include <vector>

typedef struct _string_data {
	std::string minterm;
	char flag;
} StringData;

typedef struct _logic_expression
{
	std::vector<StringData> minterm;
	int minterm_num;
	int bit_length;
}LogicExpr;

LogicExpr FileRead(const char file_name[]);