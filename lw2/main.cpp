//
//{
//<Условие задачи:
//26. Имеется  выражение  в  постфиксной  форме,   включающее
//обращение к функциям SIN, COS, EXP.  Переменные  заданы  одной
//строчной латинской буквой.  Запросить  значения  переменных  и
//вычислить выражение. Представить  его  в  инфиксной  форме  со
//скобками. Лишние скобки присутствовать не должны (11).
//>
//<Автор: Борисов Кирилл Александрович ПС-22>
//<Среда выполнения: Jetbrains CLion Runtime version: 17.0.8+7-b1000.8 amd64>
//<Источник информации: >
//}

#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cmath>

using namespace std;

string pointToComma(string val)
{
	string result {};
	for (char ch : val)
	{
		if (ch == '.')
		{
			ch = ',';
		}
		result += ch;
	}
	return result;
}

bool isNumeric(string &str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (!isdigit(str[i])
			&& !(isdigit(str[i-1]) && (str[i] == '.' || str[i] == ',') && isdigit(str[i+1])))
		{
			return false;
		}
	}
	return !str.empty();
}

struct S {
	string el {};
	S* next = nullptr;
};

void push(S *&stack, string el)
{
	S *ptr = new S;
	ptr->el = el;
	ptr->next = stack;
	stack = ptr;
}

void pop(S *&stack)
{
	S* ptr = stack;
	stack = stack->next;
	delete ptr;
}

string top(S *stack)
{
	return stack->el;
}

bool empty(S *stack)
{
	return top(stack).empty();
}

int length(S *stack)
{
	int i = 0;
	while (stack->next != nullptr)
	{
		i++;
		stack = stack->next;
	}
	return i;
}

void outputStack(S *stack)
{
	S *ptr = stack;
	cout << '[';
	bool isFirst = true;
	while (ptr && !ptr->el.empty())
	{
		if (isFirst)
		{
			isFirst = false;
		}
		else
		{
			cout << ", ";
		}
		cout << ptr->el;
		ptr = ptr->next;
	}
	cout << ']' << endl;
}

string cutExtraSpaces(string str)
{
	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		std::string::iterator begin = it;
		while (it != str.end() && ::isspace(*it) )it++;
		if (it - begin > 1)
			it = str.erase(begin + 1, it) - 1;
	}

	return str;
}

string parseExpression(string expr)
{
	string vars {};
	for (int i = 0; i < expr.length(); i++)
	{
		if ((i == 0 || expr[i - 1] == ' ')
			&& (i == expr.length() - 1 || expr[i + 1] == ' ')
			&& expr[i] >= 'a'
			&& expr[i] <= 'z')
		{
			vars += expr[i];
		}
	}

	return vars;
}

double countExpression(string expression, map<char, double> varMap)
{
	double value = 0;
	S *stack = new S;
	istringstream expr(expression);

	string token {};
	while (!expr.eof())
	{
		expr >> token;
		if (token.length() == 1
			&& token[0] >= 'a'
			&& token[0] <= 'z'
			|| isNumeric(token))
		{
			if (isNumeric(token))
			{
				push(stack, token);
			}
			else
			{
				push(stack, to_string(varMap[token[0]]));
			}
		}
		else if (token.length() == 1)
		{
			double a, b, result {};
			if (!empty(stack))
			{
				string tmpStr = pointToComma(top(stack));
				a = stod(tmpStr);
				pop(stack);
			}
			else
			{
				wstring wsTmp(token.begin(), token.end());
				wcerr << L"Недостаточно аргументов для оператора " << wsTmp << endl;
				exit(1);
			}
			if (!empty(stack))
			{
				string tmpStr = pointToComma(top(stack));
				b = stod(tmpStr);
				pop(stack);
			}
			else
			{
				wstring wsTmp(token.begin(), token.end());
				wcerr << L"Недостаточно аргументов для оператора " << wsTmp << endl;
				exit(1);
			}
			if (token[0] == '+')
			{
				result = a + b;
			}
			else if (token[0] == '-')
			{
				result = b - a;
			}
			else if (token[0] == '*')
			{
				result = a * b;
			}
			else if (token[0] == '/')
			{
				result = b / a;
			}
			else if (token[0] == '^')
			{
				result = pow(b, a);
			}
			push(stack, to_string(result));
		}
		else if (token.length() > 1)
		{
			if (token != "SIN" && token != "COS" && token != "EXP" && token != "LN"
				&& token != "sin" && token != "cos" && token != "exp" && token != "ln")
			{
				wstring wsTmp(token.begin(), token.end());
				wcerr << L"Неизвестный оператор " << wsTmp << endl;
				exit(1);
			}
			double result {};
			double a;
			if (!empty(stack))
			{
				string tmpStr = pointToComma(top(stack));
				a = stod(tmpStr);
				pop(stack);
			}
			else
			{
				wstring wsTmp(token.begin(), token.end());
				wcerr << L"Недостаточно аргументов для оператора " << wsTmp << endl;
				exit(1);
			}
			if (token == "SIN" || token == "sin")
			{
				result = sin(a);
			}
			else if (token == "COS" || token == "cos")
			{
				result = cos(a);
			}
			else if (token == "EXP" || token == "exp")
			{
				result = exp(a);
			}
			else if (token == "LN" || token == "ln")
			{
				result = log(a);
			}
			else
			{
				wstring wsTmp(token.begin(), token.end());
				wcerr << L"Неизвестный оператор " << wsTmp << endl;
				exit(1);
			}
			push(stack, to_string(result));
		}
		expr.peek();
	}

	if (!empty(stack))
	{
		if (length(stack) > 1)
		{
			wcerr << L"Выражение введено неверно" << endl;
			exit(1);
		}
		value = stod(top(stack));
	}
	else
	{
		wcerr << L"Выражение введено неверно" << endl;
		exit(1);
	}

	return value;
}

int getPriority(string operation)
{
	if (operation == "+" || operation == "-")
	{
		return 1;
	}
	else  if (operation == "*" || operation == "/")
	{
		return 2;
	}
	else if (operation == "^" || operation == "SIN" || operation == "COS" || operation == "EXP" || operation == "LN"
			|| operation == "sin" || operation == "cos" || operation == "exp" || operation == "ln")
	{
		return 3;
	}
	else
	{
		return 0;
	}
}

bool checkBrackets(string &str, string token, string prevToken)
{
	if (str.length() > 1 && !isNumeric(str))
	{
		if (token == "+" && prevToken == "-")
		{
			return false;
		}
		if (token == "+" && prevToken == "+")
		{
			return false;
		}
		if (token == "*" && prevToken == "*")
		{
			return false;
		}
		if (token == "*" && prevToken == "/")
		{
			return false;
		}
		if (getPriority(prevToken) != 0
			&& getPriority(token) >= getPriority(prevToken))
		{
			str.insert(str.begin(), '(');
			str += ')';
			return true;
		}
	}
	return false;
}

string infixingPrefix(string prefixExpr)
{
	string infix {};
	S *baseStack = new S;
	S *extraStack = new S;

	istringstream expr(prefixExpr);
	string token {};
	while (!expr.eof())
	{
		expr >> token;
		string prevToken = top(extraStack);
		if (token.length() == 1
			&& token[0] >= 'a'
			&& token[0] <= 'z'
			|| isNumeric(token))
		{
			if (prevToken == "/")
			{
				pop(extraStack);
			}
			push(baseStack, token);
		}
		else if (token.length() == 1)
		{
			string a;
			if (!empty(baseStack))
			{
				a = top(baseStack);
				pop(baseStack);
			}
			else
			{
				wstring wsTmp(token.begin(), token.end());
				wcerr << L"Недостаточно аргументов для оператора " << wsTmp << endl;
				exit(1);
			}
			string b;
			if (!empty(baseStack))
			{
				b = top(baseStack);
				pop(baseStack);
			}
			else
			{
				wstring wsTmp(token.begin(), token.end());
				wcerr << L"Недостаточно аргументов для оператора " << wsTmp << endl;
				exit(1);
			}
			string result {};
			if (token[0] == '+')
			{
				if (checkBrackets(a, token, prevToken))
				{
					if (!empty(extraStack))
					{
						pop(extraStack);
					}
				}
				result += b + " + " + a;
				push(extraStack, token);
			}
			else if (token[0] == '-')
			{
				if (checkBrackets(a, token, prevToken))
				{
					if (!empty(extraStack))
					{
						pop(extraStack);
					}
				}

				result += b + " - " + a;
				push(extraStack, token);
			}
			else if (token[0] == '*')
			{
				if (a.length() != 1)
				{
					checkBrackets(a, token, prevToken);
					if (!empty(extraStack))
					{
						pop(extraStack);
					}
				}
				if (b.length() != 1)
				{
					checkBrackets(b, token, prevToken);
					if (!empty(extraStack))
					{
						pop(extraStack);
					}
				}
				result += b + " * " + a;
				push(extraStack, token);
			}
			else if (token[0] == '/')
			{
				if (a.length() != 1)
				{
					checkBrackets(a, token, prevToken);
					if (!empty(extraStack))
					{
						pop(extraStack);
					}
				}

				prevToken = top(extraStack);
				if (prevToken != "*")
				{
					if (b.length() != 1)
					{
						checkBrackets(b, token, prevToken);
						if (!empty(extraStack))
						{
							pop(extraStack);
						}
					}
				}

				result += b + " / " + a;
				push(extraStack, token);
			}
			else if (token[0] == '^')
			{
				if (checkBrackets(a, token, prevToken))
				{
					if (!empty(extraStack))
					{
						pop(extraStack);
					}
				}
				if (checkBrackets(b, token, prevToken))
				{
					if (!empty(extraStack))
					{
						pop(extraStack);
					}
				}
				result += b + " ^ " + a;
				push(extraStack, token);
			}
			push(baseStack, result);
		}
		else if (token.length() > 1)
		{
			string result {};
			string a;
			if (!empty(baseStack))
			{
				a = top(baseStack);
				pop(baseStack);
			}
			else
			{
				wstring wsTmp(token.begin(), token.end());
				wcerr << L"Недостаточно аргументов для оператора " << wsTmp << endl;
				exit(1);
			}
			if (token == "SIN" || token == "sin")
			{
				if (checkBrackets(a, token, prevToken))
				{
					pop(extraStack);
					result = token + a;
				}
				else
				{
					result = token + " " + a;
				}
				push(extraStack, token);
			}
			else if (token == "COS" || token == "cos")
			{
				if (checkBrackets(a, token, prevToken))
				{
					pop(extraStack);
					result = token + a;
				}
				else
				{
					result = token + " " + a;
				}
				push(extraStack, token);
			}
			else if (token == "EXP" || token == "exp")
			{
				if (checkBrackets(a, token, prevToken))
				{
					pop(extraStack);
					result = token + a;
				}
				else
				{
					result = token + " " + a;
				}
				push(extraStack, token);
			}
			else if (token == "LN" || token == "ln")
			{
				if (checkBrackets(a, token, prevToken))
				{
					pop(extraStack);
					result = token + a;
				}
				else
				{
					result = token + " " + a;
				}
				push(extraStack, token);
			}
			else
			{
				wstring wsTmp(token.begin(), token.end());
				wcerr << L"Неизвестный оператор " << wsTmp << endl;
				exit(1);
			}
			push(baseStack, result);
		}
	}

	infix = top(baseStack);
	return infix;
}

int main()
{
	//TODO: a b c - - | a - (b - c)
	//TODO: a b ^ c ^ | (a^b) ^ c
	//TODO: a b c / * | a * b / c
	//TODO: a b c / / | a / (b / c)

	setlocale(0, "");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	wcout << L"Введитеe выражение в постфиксной форме: ";
	string expression;
	char first = cin.get();
	if (first == '\n')
	{
		wcerr << L"Ошибка ввода" << endl;
		exit(1);
	}
	getline(cin, expression);
	expression.insert(expression.begin(), first);
	expression = cutExtraSpaces(expression);

	string variables = parseExpression(expression);
	map<char, double> varMap;
	if (!variables.empty())
	{
		//TODO: Вынести в функцию
		wcout << L"Введите значения переменных:" << endl;
		for (char var : variables)
		{
			if (varMap.find(var) != varMap.end())
			{
				continue;
			}

			string val;
			bool isError = false;
			do
			{
				if (isError)
				{
					wcerr << L"Введите число" << endl;
				}
				isError = true;
				wcout << var << L" = ";
				cin >> val;
			}
			while (!isNumeric(val));

			val = pointToComma(val);
			varMap[var] = stod(val);
		}
	}

	double value = countExpression(expression, varMap);
	if (int(value) == value)
	{
		wcout << L"Значение выражения: " << to_wstring(int(value)) << endl;
	}
	else
	{
		wcout << L"Значение выражения: " << to_wstring(value) << endl;
	}

	string infix = infixingPrefix(expression);
	wstring wsTmp(infix.begin(), infix.end());
	wcout << L"Инфиксная запись: " << wsTmp << endl;

	return 0;
}