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

typedef struct _pi_s {
	std::string PI;
	std::vector<std::string> true_minterm;
} PIs;

bool ComparePIs(std::string pi, std::string minterm, int bit_length);
LogicExpr FileRead(const char file_name[]);
std::vector<PIs> MakeImplicantTable(LogicExpr& logic_expr);
bool CompareMinterm(const StringData& data1, const StringData& data2);
int FindHamOne(const std::string& str1, const std::string& str2, int bit_length);
int FindHot(std::string minterm, int bit_length);