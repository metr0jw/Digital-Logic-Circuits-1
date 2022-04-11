#include <iostream>
#include <vector> 
#include "findpi.h"

/***************************************************************
*Logic Expression : Don't Care = 2 ex) not appeared in minterm *	
*					     FALSE = 0 ex) A`					   *
*					      TRUE = 1 ex) A					   *
****************************************************************/
int FindCost(const std::vector<std::string>& logic_expression, const int bit_length)
{	//TODO : parameter�� string&�� �ƴ� ���ɼ� ����.
	//logic_expression : ���Ŀ��� true minterm�鸸 �����ؼ� ������ ����
	//bit_length : �ϳ��� minterm���� ������ �� �ִ� �ִ� bit��
	const std::string const* logic_expr_ptr = logic_expression.data();
	int cost = 0;
	int minterm_num = 0;
	int and_gate_cost = 0;
	int or_gate_cost = 0;
	int not_gate_cost = 0;
	

	char* is_not = new char[bit_length];
	for (int i = 0; i < bit_length; i++) is_not[i] = 0; // �ʱ�ȭ
	//for (std::vector<std::string>::const_iterator iter = logic_expression.begin(); iter != logic_expression.end(); iter++, minterm_num++);
	minterm_num = logic_expression.size();
	//vector ������ minterm ���� ���ϱ�
	//TODO : �̰� �ȱ��ϰ� vector size�� ���ص� �ɵ�? (SOLVED)
	std::cout << minterm_num << "\n";
	
	if (minterm_num > 1)	//minterm�� 2�� �̻��϶����� N-input OR Gate
		or_gate_cost = 6 + (2 * (minterm_num - 2));	//or gate ���� ���ϱ�

	for (int i = 0; i < minterm_num; i++)
	{
		int literal_num = 0;
		for (int count = 0; count < bit_length; count++)
		{
			if (logic_expr_ptr[i][count] != '2')	//product term�� literal ���� ���ϱ�
			{
				literal_num++;
				is_not[count] = 1;	//NOT gate ���� ���ϱ�
			}

		}
		if (literal_num > 1) //literal�� 2�� �̻��϶����� N-input AND Gate
			and_gate_cost += 6 + (2 * (literal_num - 2));	//and gate ���� ���ϱ�
	}
	for (int i = 0; i < bit_length; i++)
		if (is_not[i] == 1) not_gate_cost += 2;	//not gate ���� ���ϱ�

	/* Debug Only */
	std::cout << "and : " << and_gate_cost << "\n";
	std::cout << "or : " << or_gate_cost << "\n";
	std::cout << "not : " << not_gate_cost << "\n";
	/*************/
	cost = and_gate_cost + or_gate_cost + not_gate_cost;

	delete[] is_not;
	return cost;
}

/********FindCost TestCase**********/
/*

	std::vector<std::string> logic_expression;
	//A`B + AC`D +AB`CD`
	logic_expression.emplace_back("0122");
	logic_expression.emplace_back("1201");
	logic_expression.emplace_back("1010");
*/