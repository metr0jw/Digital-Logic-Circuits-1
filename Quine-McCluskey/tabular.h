#pragma once
#include <iostream>
#include <string>
#include <vector>

typedef struct _string_data {
	std::string minterm;
	char flag;
} StringData;	//Minterm�� ������ �ľ�. Don'tcare minterm���� true minterm����

typedef struct _logic_expression
{
	std::vector<StringData> minterm;
	int minterm_num;
	int bit_length;
}LogicExpr;	//������ ������ ����ü. ��Ʈ�� ���̿� minterm�� ���� ����

typedef struct _pi_s {
	std::string PI;	
	std::vector<std::string> true_minterm;
} PIs;	//Prime Implicant�� �ű⿡ �����Ǵ� True Minterm���� ����

bool ComparePIs(std::string pi, std::string minterm, int bit_length);
LogicExpr FileRead(const char file_name[]);
std::vector<PIs> MakeImplicantTable(LogicExpr& logic_expr);
int FindHamOne(const std::string& str1, const std::string& str2, int bit_length);
int FindHot(std::string minterm, int bit_length);