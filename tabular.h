#pragma once
#include <iostream>
#include <string>
#include <vector>

typedef struct _string_data {
	std::string minterm;
	char flag;
	int num_of_hot;
} StringData;

typedef struct _logic_expression
{
	std::vector<StringData> minterm;
	int minterm_num;
	int bit_length;
}LogicExpr;

LogicExpr FileRead(const char file_name[]);
std::vector<std::string> MakeImplicantTable(LogicExpr& logic_expr);
int FindHot(std::string minterm, int bit_length);
bool CompareMinterm(const StringData& data1, const StringData& data2);
int FindHamOne(const std::string& str1, const std::string& str2, int bit_length);