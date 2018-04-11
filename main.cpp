#include <iostream>
#include <unordered_map>
#include <string>
#include<vector>
#include<cmath>
#include <fstream>
bool delim (char c) {
	return c == ' ';
}

bool is_op (char c) {
	return c=='&' || c=='|';
}

int priority (char op) {
	return (op == '|') ? 1 : (op == '&') ? 2 : -1;
}

void process_op (std::vector<int> & st, char op) {
	int r = st.back();  st.pop_back();
	int l = st.back();  st.pop_back();
	switch (op) {
		case '|':  st.push_back(l | r);  break;
		case '&':  st.push_back(l & r);  break;
		default: break;
	}
}

int calc (std::string & s, std::unordered_map<std::string,int> variables) {
	std::vector<int> st;
	std::vector<char> op;
	for (size_t i=0; i<s.length(); ++i)
		if (!delim (s[i]))
			if (s[i] == '(')
				op.push_back ('(');
			else if (s[i] == ')') {
				while (op.back() != '(')
					process_op (st, op.back()),  op.pop_back();
				op.pop_back();
			}
			else if (is_op (s[i])) {
				char curop = s[i];
				while (!op.empty() && priority(op.back()) >= priority(s[i]))
					process_op (st, op.back()),  op.pop_back();
				op.push_back (curop);
			}
			else {
				std::string operand;
				while (i < s.length() && isalnum (s[i]))
				operand += s[i++];
				--i;
				if (isdigit (operand[0]))
					st.push_back (atoi(operand.c_str()));
				else
				{
					//TODO
					//find in map exist variables
					if(variables.count(operand)<=0)
						return -1;
					st.push_back(variables[operand]);
				}
			}
	while (!op.empty())
		process_op (st, op.back()),  op.pop_back();
	return st.back();
}

int main(int argc, char *argv[])
{

	std::ifstream in(argv[0]);
	std::ofstream out(argv[1]);
	std::unordered_map<std::string,int> variables;
	int n,k,temp_int;
	in>>n>>k;
	std::string temp_str;
	for(int i=0;i<n;i++)
	{
		in>>temp_str>>temp_int;
		variables[temp_str]=temp_int;
	}
	std::string expression;
	std::getline(in,expression);
	expression.clear();
	for(int i=0;i<k;i++)
	{
		std::getline(in,expression);
		unsigned long eq_num = expression.find('=');
		temp_str = expression.substr(0,eq_num-1);
		std::string logic_expression = expression.substr(eq_num+1, (expression.size()-eq_num));
		temp_int = calc(logic_expression,variables);
		if(temp_int == -1)
			out<<"?\n";
		else
			out<<temp_int<<"\n";
		variables[temp_str]=temp_int;
		expression.clear();
	}
	return 0;
}