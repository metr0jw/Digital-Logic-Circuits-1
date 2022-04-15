#pragma once
#include <iostream>
#include <string>
#include <vector>

typedef struct _string_data {
	std::string minterm;
	char flag;
} StringData;	//Minterm의 종류를 파악. Don'tcare minterm인지 true minterm인지

typedef struct _logic_expression
{
	std::vector<StringData> minterm;
	int minterm_num;
	int bit_length;
}LogicExpr;	//논리식을 저장할 구조체. 비트의 길이와 minterm의 개수 저장

typedef struct _pi_s {
	std::string PI;	
	std::vector<std::string> true_minterm;
} PIs;	//Prime Implicant와 거기에 대응되는 True Minterm들을 저장

bool ComparePIs(std::string pi, std::string minterm, int bit_length);
LogicExpr FileRead(const char file_name[]);
std::vector<PIs> MakeImplicantTable(LogicExpr& logic_expr);
int FindHamOne(const std::string& str1, const std::string& str2, int bit_length);
int FindHot(std::string minterm, int bit_length);