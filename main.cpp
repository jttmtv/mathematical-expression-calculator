#include <bits/stdc++.h>

using namespace std;

inline int Input_int();//check the validity of the input of the integer variable
inline void Trim(string&);//remove all spaces in the string
inline void Analyze(vector<string>, string&, int);//analyze mathematical expressions containing unknown numbers
vector<string> Breakup(string);//detachment expression
stack<string> Infix_to_suffix(vector<string>);//Turn infix expressions into postfix expressions
inline bool Judge_infix(string);//Determine the legality of the infix expression
int right_loc(string, int);//Returns the position of the right parenthesis that matches the specified left parenthesis in the infix expression
inline string replace_all_distinct(string, const string, const string);//Replace all specified substrings in the parent string

inline int Base_compare(string, string);//Compare the size of two long integers
inline int Compare(string, string);//Compare the size of ultra-large and ultra-high-precision floating-point numbers, return 0 if equal, return 1, greater than return, and return -1 if less than
inline bool Check(string);//Check if the operand is a floating point number
inline int Negative(string, string);//Determine the sign of the left and right operands
vector<string> Decimal_get(string, bool);//Returns the integer and fractional part of a floating point number

string Sum(string, string);//  +full precision
string Sub(string, string);//  -full precision
string Mul(string, string);//  *full precision
string Div(string, string);//  /double precision

string mul(string, string);//Realize large integer multiplication operations
string Math_functions(string);//Mathematical functions
string Computation(stack<string>);//Core function, calculate the value of suffix expression

int main()
{
	vector<string> equation;
	string str;
	int au;
	cout << "Legal expressions look like as follows!\n"
		<< endl;
	cout << "x=3\ny=7\n((x+2)*(y%3))-sqrt(x*y-10)\n"
		<< endl;
	cout << "supported mathematical functions:sqrt(x),sin(x),cos(x),ln(x),|x|\n"
		<< endl;
	while (true)
	{
		cout << "please enter the number of unknowns :" << endl;
		au = Input_int();
		cout << "enter the expression to be calculated :" << endl;

		for (int i = 0; i < au; i++)
		{
			getline(cin, str);
			Trim(str);
			equation.push_back(str);
		}
		getline(cin, str);
		Trim(str);
		Analyze(equation, str, au);
		str = Math_functions(str);
		if (str == "false") {}
		else cout << str << endl;
		cout << "continue caculation? Y:1 N:0" << endl;
		au = Input_int();
		if (au == 0)
			break;
	}
	system("pause");
	return 0;
}

bool Judge_infix(const string str)
{
	int l = str.length();
	if (!isdigit(str[0]) && str[0] != '(' && str[0]!='-')
		return false;  //The first place is not (nor is it a number, illegal
	if (!isdigit(str[l - 1]) && str[l - 1] != ')')
		return false;  //The last digit is neither) nor a number, illegal
	int now = 0;  //Brackets do not match, illegal 
	for (int i = 0; i < l; i++)
	{
		if (str[i] == '(')
			now++;
		else if (str[i] == ')')
			now--;
		if (now < 0)
			return false;
	}
	if (now != 0)
		return false;
	for (int i = 1; i < l - 1; i++)
	{
		if (str[i] == ' + ' || str[i] == ' * ' || str[i] == ' / ' || str[i] == ' - ' || str[i] == '%')  //Binary operator
		{
			if (!isdigit(str[i - 1]) && str[i - 1] != ')')  //It’s not on the left) or a number, illegal 
				return false;
			if (!isdigit(str[i + 1]) && str[i + 1] != '(')  //右边不是(也不是数字，不合法 
				return false;
		}
		if (str[i] == '.' && !isdigit(str[i - 1]) && !isdigit(str[i + 1]))
			return false;  //The left and right points are not numbers, illegal
	}
	for (int i = 1; i < l; i++)  //The decimal point is expanded to the left and right. Before encountering an operator, only a number can be expanded to be legal
	{
		if (str[i] == '.')
		{
			int j = i - 1, k = i + 1;
			while (j > 0 && isdigit(str[j])) j--;
			while (k < l && isdigit(str[k])) k++;
			if (str[j] == '.' || str[k] == '.')
				return false;
		}
	}
	return true;
}


inline void Trim(string& s) //Remove spaces from the string
{
	int index = 0;
	if (!s.empty())
	{
		while ((index = s.find(' ', index)) != string::npos)
		{
			s.erase(index, 1);
		}
	}
}

inline void Analyze(vector<string> vec_str, string& str, int cnt)
{
	vector<int> loc;
	string str1, str2;
	for (int i = 0; i < cnt; i++)
	{
		loc.push_back(vec_str[i].find('='));
	}
	for (int i = 0; i < cnt; i++)
	{
		str1 = vec_str[i].substr(0, loc[i]);
		str2 = vec_str[i].substr(loc[i] + 1);
		str = replace_all_distinct(str, str1, str2);
	}
}

inline string replace_all_distinct(string str, const string old_value, const string new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length())
	{
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return str;
}


//check input legitimacy
inline int Input_int()
{
	string x;
	getline(cin, x);
	while (true)
	{
		bool flag = false;
		for (auto ch : x)
		{
			if (ch < '0' || ch>'9')
			{
				flag = true;
				break;
			}
		}
		cin.clear(); //Clear flow mark
		cin.sync();  //Empty stream
		if (flag)
		{
			cout << "illegal input ,try agin!" << endl;
			getline(cin, x);
		}
		else
			break;
	}
	return stoi(x);
}


int right_loc(string str, int loc_begin)
{
	stack<char> bru;//Stack of brackets
	for (int i = loc_begin; i < str.length(); i++)
	{
		if (str[i] == '(')
		{
			bru.push(str[i]);
		}
		if (str[i] == ')')
		{
			bru.pop();
		}
		if (bru.empty())
		{
			return ++i;
		}
	}
}


string Math_functions(string str)
{
	string::size_type loc_begin, loc_end;
	string temp;
	if (str.find("sqrt") == string::npos && str.find("sin") == string::npos && str.find("cos") == string::npos && str.find("ln") == string::npos && str.find("|") == string::npos)
	{
		if (!Judge_infix(str))
		{
			cout << "the expression is invalid!" << endl;
			return "false";
		}
		else
			return Computation(Infix_to_suffix(Breakup(str)));
	}
	else
	{
		while (str.find("sqrt") != string::npos || str.find("sin") != string::npos || str.find("cos") != string::npos || str.find("ln") != string::npos || str.find("|") != string::npos)
		{
			if (str.find("sqrt") != string::npos)
			{
				loc_begin = str.find("sqrt");
				loc_end = right_loc(str, loc_begin + 4);
				temp = str.substr(loc_begin, loc_end - loc_begin);
				temp = to_string(sqrt(stod(Math_functions(temp.substr(5, temp.length() - 6)))));
				str = str.replace(loc_begin, loc_end - loc_begin, temp);

			}

			if (str.find("sin") != string::npos)
			{
				loc_begin = str.find("sin");
				loc_end = right_loc(str, loc_begin + 3);
				temp = str.substr(loc_begin, loc_end - loc_begin);
				temp = to_string(sin(stod(Math_functions(temp.substr(4, temp.length() - 5)))));
				str = str.replace(loc_begin, loc_end - loc_begin, temp);

			}

			if (str.find("cos") != string::npos)
			{
				loc_begin = str.find("cos");
				loc_end = right_loc(str, loc_begin + 3);
				temp = str.substr(loc_begin, loc_end - loc_begin);
				temp = to_string(cos(stod(Math_functions(temp.substr(4, temp.length() - 5)))));
				str = str.replace(loc_begin, loc_end - loc_begin, temp);

			}

			if (str.find("ln") != string::npos)
			{
				loc_begin = str.find("ln");
				loc_end = right_loc(str, loc_begin + 2);
				temp = str.substr(loc_begin, loc_end - loc_begin);
				temp = to_string(log(stod(Math_functions(temp.substr(3, temp.length() - 4)))));
				str = str.replace(loc_begin, loc_end - loc_begin, temp);

			}
		}
	}
	return str;
}

vector<string> Breakup(string str)
{
	vector<string> Vecstr;
	regex pattern("[[:digit:].]"); //matching using regular expressions
	string str1, str2, str3;
	for (auto i = str.begin(); i != str.end(); ++i)
	{
		str2 = *i;
		if (regex_search(str2, pattern))
		{
			str1 += str2;
			auto p = i;

			if (++p == str.end())
			{
				Vecstr.push_back(str1);
				str1.clear();
			}
			--p;
			if (++p != str.end())
			{
				--p;
				str2 = *(++p);

				if (!regex_search(str2, pattern))
				{
					Vecstr.push_back(str1);
					str1.clear();
				}
			}
		}
		else
		{
			if (str2 == "+" || str2 == "-" || str2 == "*" || str2 == "/" || str2 == "(" || str2 == "%")
			{
				Vecstr.push_back(str2);
				auto p = i;
				if (++p != str.end())
				{
					--p;
					str3 = *(++p);
					if (str3 == "-")
					{
						str1 += str3;
						if (++p != str.end())
						{
							auto p1 = p;
							str3 = *p1;
							for (; regex_search(str3, pattern) && p1 != str.end(); ++p1, ++i)
							{
								str3 = *p1;
								if (regex_search(str3, pattern))
								{
									str1 += str3;
								}
							}
							Vecstr.push_back(str1);
							str1.clear();
							if (regex_search(str3, pattern))
							{
								++i;
							}
						}
					}
				}
			}
			else
			{
				Vecstr.push_back(str2);
			}
		}
	}
	return Vecstr;
}

stack<string> Infix_to_suffix(vector<string> Vecstr)
{
	stack<string> sym, Exp, stack1, stack2;
	regex pattern("[[:digit:].]+");
	string str;

	for (auto& i : Vecstr)
	{
		str = i;

		if (regex_search(str, pattern))
		{
			Exp.push(str);
		}

		if (str == "(")
		{
			sym.push(str);
		}

		if (str == ")")
		{
			while (sym.top() != "(")
			{
				Exp.push(sym.top());
				sym.pop();
			}
			sym.pop();
		}

		if (str == "+" || str == "-")
		{
			if (!sym.empty())
			{
				if (sym.top() == "+" || sym.top() == "-" || sym.top() == "*" || sym.top() == "/" || sym.top() == "%")
				{
					while (!sym.empty() && sym.top() != "(")
					{
						Exp.push(sym.top());
						sym.pop();
					}
					sym.push(str);
				}
				else
				{
					sym.push(str);
				}
			}
			else
			{
				sym.push(str);
			}
		}

		if (str == "%")
		{
			if (!sym.empty())
			{
				if (sym.top() == "*" || sym.top() == "/" || sym.top() == "%")
				{
					while (!sym.empty() && sym.top() != "(" && sym.top() != "+" && sym.top() != "-")
					{
						Exp.push(sym.top());
						sym.pop();
					}
					sym.push(str);
				}
				else
				{
					sym.push(str);
				}
			}
			else
			{
				sym.push(str);
			}
		}

		if (str == "*" || str == "/")
		{
			if (!sym.empty())
			{
				if (sym.top() == "*" || sym.top() == "/")
				{
					while (!sym.empty() && sym.top() != "(" && sym.top() != "+" && sym.top() != "-")
					{
						Exp.push(sym.top());
						sym.pop();
					}
					sym.push(str);
				}
				else
				{
					sym.push(str);
				}
			}
			else
			{
				sym.push(str);
			}
		}
	}
	while (!sym.empty())
	{
		Exp.push(sym.top());
		sym.pop();
	}
	while (!Exp.empty())
	{
		stack1.push(Exp.top());
		stack2.push(Exp.top());
		Exp.pop();
	}
	return stack1;
}

//Caculate the value
string Computation(stack<string> Exp)
{
	string num1, num2;
	regex pattern("[[:digit:].]+");
	stack<string> satcknum;
	string str;
	try
	{
		while (!Exp.empty())
		{
			str = Exp.top();

			if (regex_search(str, pattern))
			{
				satcknum.push(str);
				Exp.pop();
			}

			if (str == "+")
			{
				if (!satcknum.empty())
				{
					num1 = satcknum.top();
					satcknum.pop();
					num2 = satcknum.top();
					satcknum.pop();
					satcknum.push(Sum(num1, num2));
					Exp.pop();
				}
				else
				{
					throw runtime_error("Expression input error, please re-enter!");
				}
			}

			if (str == "-")
			{
				if (!satcknum.empty())
				{
					num1 = satcknum.top();
					satcknum.pop();
					num2 = satcknum.top();
					satcknum.pop();
					satcknum.push(Sub(num2, num1));
					Exp.pop();
				}
				else
				{
					throw runtime_error("Expression input error, please re-enter!");
				}
			}

			if (str == "%")
			{
				if (!satcknum.empty())
				{
					num1 = satcknum.top();
					satcknum.pop();
					num2 = satcknum.top();
					satcknum.pop();
					satcknum.push(to_string((fmod(stod(num2), stod(num1)))));
					Exp.pop();
				}
				else
				{
					throw runtime_error("Expression input error, please re-enter!");
				}
			}

			if (str == "*")
			{
				if (!satcknum.empty())
				{
					num1 = satcknum.top();
					satcknum.pop();
					num2 = satcknum.top();
					satcknum.pop();
					satcknum.push(Mul(num1, num2));
					Exp.pop();
				}
				else
				{
					throw runtime_error("Expression input error, please re-enter!");
				}
			}

			if (str == "/")
			{
				if (!satcknum.empty())
				{
					num1 = satcknum.top();
					satcknum.pop();
					num2 = satcknum.top();
					satcknum.pop();
					satcknum.push(to_string((stod(num2) / stod(num1))));
					Exp.pop();
				}
				else
				{
					throw runtime_error("Expression input error, please re-enter!");
				}
			}
		}
	}
	catch (runtime_error& err)
	{
		cout << err.what() << endl;
	}
	return satcknum.top();
}

inline bool Check(string num)
{
	bool flag = false;
	for (auto c : num)
	{
		if (!isdigit(c) || num.empty())
		{
			if (c == '.' && !flag)
			{
				flag = true;
			}
			else
			{
				cout << "num1: Please input correct form!!!" << endl;
				return false;
			}
		}
	}
	return flag;
}


vector<string> Decimal_get(string num, bool flag)
{
	vector<string> num_split;
	string temp1, temp2;
	if (flag) //If it is a decimal
	{
		int i = 0;
		while (i != num.length() && num[i] != '.')
		{
			temp1 += num[i];
			++i;
		}
		while (++i != num.length())
		{
			temp2 += num[i];
		}
		//Used for the control of .xxx or xxx. type input
		if (temp1.empty())
			temp1 += '0';
		if (temp2.empty())
			temp2 += '0';
	}
	else //If it is an integer
	{
		temp1 = num;
		temp2 += '0';
	}
	num_split.push_back(temp1);
	num_split.push_back(temp2);
	return num_split;
}


inline int Negative(string str1, string str2)//Determine the sign of the left and right operands
{
	if (str1[0] == '-')
	{
		if (str2[0] == '-')//--
		{
			return 1;
		}
		else//-+
		{
			return 2;
		}
	}
	else
	{
		if (str2[0] == '-')//+-
		{
			return 3;
		}
		else//++
		{
			return 4;
		}
	}
}

inline int Base_compare(string str1, string str2)   //Compare the size of the string type "number", return 0 if it is equal, return 1 if it is greater, and return -1 if it is less than
{
	if (str1.length() > str2.length())
		return 1;
	else if (str1.length() < str2.length())
		return -1;
	else                                           //If the lengths are equal, compare the bits from beginning to end
		return str1.compare(str2);
}

inline int Compare(string str1, string str2)   /*Compare the size of ultra-large and ultra-high-precision floating-point numbers, return 0 if equal, return 1, greater than return, and return -1 if less than*/
{
	if (!Check(str1))
		str1 += ".0";
	if (!Check(str2))
		str2 += ".0";
	string temp[4];

	int i = 0;

	while (i != str1.length() && str1[i] != '.')//Separate the decimal and integer of str1
	{
		temp[0] += str1[i];
		++i;
	}
	while (++i != str1.length())
	{
		temp[1] += str1[i];
	}

	i = 0;
	while (i != str2.length() && str2[i] != '.')//Separate the decimal and integer of str2
	{
		temp[2] += str2[i];
		++i;
	}
	while (++i != str2.length())
	{
		temp[3] += str2[i];
	}

	if (Base_compare(temp[0], temp[2]) >= 0)
	{
		if (Base_compare(temp[0], temp[2]) > 0)
			return 1;
		else
		{
			if (Base_compare(temp[1], temp[3]) >= 0)
			{
				if (Base_compare(temp[1], temp[3]) > 0)
					return 1;
				else
					return 0;
			}
			else
				return -1;
		}
	}
	else
		return -1;
}


//Plus
string Sum(string str1, string str2)
{
	bool posorneg = false;//Signs of positive and negative numbers
	switch (Negative(str1, str2))
	{
	case 1:
		str1.erase(0, 1);
		str2.erase(0, 1);
		posorneg = true;//negative number
		break;
	case 2:
		str1.erase(0, 1);
		return Sub(str2, str1);
		break;
	case 3:
		str2.erase(0, 1);
		return Sub(str1, str2);
		break;
	default:
		break;
	}
	vector<string> str[2];
	bool flag1, flag2;
	flag1 = Check(str1);
	flag2 = Check(str2);
	str[0] = Decimal_get(str1, flag1);
	str[1] = Decimal_get(str2, flag2);

	//Decimal part calculation
	if (str[0][1].length() < str[1][1].length())
	{
		string temp = str[0][1];
		str[0][1] = str[1][1];
		str[1][1] = temp;
	}
	//Fill 0
	for (int i = str[1][1].length(); i < str[0][1].length(); ++i)
		str[1][1] += '0';
	//Reverse string

	reverse(str[0][1].begin(), str[0][1].end());
	reverse(str[1][1].begin(), str[1][1].end());

	//Add the fractional part
	string res2(str[0][1].length(), '0');
	int carry = 0; //carrying
	for (int i = 0; i != str[0][1].length(); ++i)
	{
		int a = str[1][1][i] - '0', b = str[0][1][i] - '0';
		a = a + b + carry;
		carry = a / 10;
		res2[i] = (a % 10) + '0';
	}
	//Reverse back
	reverse(res2.begin(), res2.end());

	//Integer part calculation
	if (str[0][0].length() < str[1][0].length())
	{
		string temp = str[0][0];
		str[0][0] = str[1][0];
		str[1][0] = temp;
	}

	//Reverse string
	reverse(str[0][0].begin(), str[0][0].end());
	reverse(str[1][0].begin(), str[1][0].end());

	string res1(str[0][0].length(), '0');
	for (int i = 0; i != str[1][0].length(); ++i)
	{
		int a = str[1][0][i] - '0', b = str[0][0][i] - '0';
		a = a + b + carry;
		carry = a / 10;
		res1[i] = (a % 10) + '0';
	}
	for (int i = str[1][0].length(); i != str[0][0].length(); ++i)
	{
		int b = str[0][0][i] - '0';
		b += carry;
		carry = b / 10;
		res1[i] = b % 10 + '0';
	}
	if (carry)
	{
		res1 += (carry + '0');
	}


	//Reverse back
	reverse(res1.begin(), res1.end());

	//Combine integer part and decimal part
	string res = res1 + '.' + res2;
	if (posorneg)
		res.insert(0, 1, '-');
	return res;
}

string Sub(string str1, string str2)
{
	bool posorneg = false;//Signs of positive and negative numbers
	switch (Negative(str1, str2))
	{
	case 1:
		str1.erase(0, 1);
		str2.erase(0, 1);
		swap(str1, str2);
		break;
	case 2:
		str2.insert(0, 1, '-');
		return Sum(str1, str2);
		break;
	case 3:
		str2.erase(0, 1);
		return Sum(str1, str2);
		break;
	default:
		break;
	}

	if (Compare(str1, str2) < 0)
	{
		swap(str1, str2);
		posorneg = true;
	}

	vector<string> str[2];
	bool flag1, flag2;
	flag1 = Check(str1);
	flag2 = Check(str2);

	str[0] = Decimal_get(str1, flag1);
	str[1] = Decimal_get(str2, flag2);

	//Subtract the decimal part
	int cnt = str[0][1].length() - str[1][1].length();

	//Fill 0
	if (cnt <= 0)
	{
		for (int i = cnt; i != 0; ++i)
		{
			str[0][1] += '0';
		}
	}

	else
	{
		for (int i = cnt; i != 0; --i)
		{
			str[1][1] += '0';
		}
	}

	//Reverse string
	reverse(str[0][1].begin(), str[0][1].end());
	reverse(str[1][1].begin(), str[1][1].end());
	string res2(str[0][1].length(), '0');
	int carry = 0;//carrying
	for (int i = 0; i != str[0][1].length(); ++i)
	{
		int a = str[0][1][i] - '0', b = str[1][1][i] - '0';
		a = a - b - carry;
		if (a >= 0)
		{
			carry = 0;
			res2[i] = a + '0';
		}
		else
		{
			carry = 1;
			res2[i] = a + 10 + '0';
		}
	}

	//Reverse back
	reverse(res2.begin(), res2.end());

	//Subtract the integer part
	//Reverse string
	reverse(str[0][0].begin(), str[0][0].end());
	reverse(str[1][0].begin(), str[1][0].end());
	string res1(str[0][0].length(), '0');
	for (int i = 0; i != str[1][0].length(); ++i)
	{
		int a = str[0][0][i] - '0', b = str[1][0][i] - '0';
		a = a - b - carry;
		if (a >= 0)
		{
			carry = 0;
			res1[i] = a + '0';
		}
		else
		{
			carry = 1;
			res1[i] = a + 10 + '0';
		}
	}

	for (int i = str[1][0].length(); i != str[0][0].length(); ++i)
	{
		int a = str[0][0][i] - '0';
		a = a - carry;
		if (a >= 0)
		{
			carry = 0;
			res1[i] = a + '0';
		}
		else
		{
			carry = 1;
			res1[i] = a + 10 + '0';
		}
	}

	//Reverse back
	reverse(res1.begin(), res1.end());

	//Clear redundancy 0
	string temp;
	cnt = 0;
	for (int i = 0; res1[i] == '0'; ++i)++cnt;
	for (int i = cnt; i != res1.length(); ++i)
		temp += res1[i];
	res1 = temp;
	if (res1.empty())
		res1 += '0';
	//Combine integer part and decimal part
	string res = res1 + '.' + res2;
	if (posorneg)
		res.insert(0, 1, '-');
	return res;
}

string mul(string str1, string str2)
{
	vector<int> res_int(str1.size() + str2.size(), 0);
	string::size_type i, j;
	vector<int>::size_type k, p;
	reverse(str1.begin(), str1.end());
	reverse(str2.begin(), str2.end());
	for (i = 0; i != str1.size(); ++i)
		for (j = 0; j != str2.size(); ++j)
			res_int[i + j] += (str1[i] - '0') * (str2[j] - '0');

	for (k = 0; k != res_int.size() - 1; ++k) {
		res_int[k + 1] += res_int[k] / 10;
		res_int[k] = res_int[k] % 10;
	}

	for (p = res_int.size() - 1; p != -1; --p) {
		if (res_int[p] != 0)
			break;
	}

	if (p == -1)
		p = 0;
	string res_str(p + 1, '0');
	for (k = p; k != -1; --k)
		res_str[p - k] = char(res_int[k] + '0');
	return res_str;
}

string Mul(string str1, string str2)
{
	bool posorneg = false;//Signs of positive and negative numbers
	switch (Negative(str1, str2))
	{
	case 1:
		str1.erase(0, 1);
		str2.erase(0, 1);
		break;
	case 2:
		str1.erase(0, 1);
		posorneg = true;
		break;
	case 3:
		str2.erase(0, 1);
		posorneg = true;
		break;
	default:
		break;
	}

	string::size_type loc1 = str1.find(".");
	if (loc1 == string::npos)
	{
		str1 += ".0";
		loc1 = str1.find(".");
	}
	str1.erase(loc1, 1);

	string::size_type loc2 = str2.find(".");

	if (loc2 == string::npos)
	{
		str2 += ".0";
		loc2 = str2.find(".");
	}
	str2.erase(loc2, 1);

	string::size_type len = (str1.size() - loc1) + (str2.size() - loc2);
	string res = mul(str1, str2);

	if (res.size() < len + 1)
		res = string(len + 1 - res.size(), '0') + res;
	res.insert(res.size() - len, ".");

	string::size_type tail = res.find_last_not_of("0");
	res = res.substr(0, tail + 1);
	if (res[res.size() - 1] == '.')
		res.erase(res.size() - 1);
	if (posorneg)
		res.insert(0, 1, '-');
	return res;
}



