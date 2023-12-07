//{
//<Условие задачи:
// 25. На диске имеется база  данных  по  физическим  эффектам
//(ФЭ).  Каждый  ФЭ  записан в виде тройки компонент:  название,
//входная  физическая  величина  (вход),   выходная   физическая
//величина  (выход).  Имеется  не более 100 различных физических
//величин. Требуется построить в основной памяти граф связи ФЭ и
//обеспечить  поиск  цепочек  ФЭ от одного до четырех звеньев по
//заданным входу и выходу по возрастанию количества звеньев(12).
//>
//<Автор: Борисов Кирилл Александрович ПС-22 />
//<Среда выполнения: Jetbrains CLion Runtime version: 17.0.8+7-b1000.8 amd64 />
//<Источник информации: />
//}
#include <windows.h>
#include <iostream>
#include <fstream>
#include <ios>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

struct EffectNode
{
	std::string name{};
	std::vector<int> input{};
	int output{};
};

// Рекурсивная функция для вставки элемента в конец заданного stack
void insertAtBottom(stack<int>& s, int item)
{
	// базовый случай: если stack пуст, вставить данный элемент в конец
	if (s.empty())
	{
		s.push(item);
		return;
	}

	// Извлечь все элементы из стека и сохранить их в стеке вызовов
	int top = s.top();
	s.pop();
	insertAtBottom(s, item);

	// После развертывания рекурсии помещаем каждый элемент в стек вызовов
	// на вершине stack
	s.push(top);
}

// Рекурсивная функция для обращения заданного stack
void reverseStack(stack<int>& s)
{
	// базовый случай: stack пуст
	if (s.empty())
	{
		return;
	}

	// Извлечь все элементы из стека и сохранить их в стеке вызовов
	int item = s.top();
	s.pop();
	reverseStack(s);

	// После развертывания рекурсии вставляем каждый элемент в стек вызовов
	// внизу stack
	insertAtBottom(s, item);
}

stack<int> outputChain(const std::vector<std::vector<int>>& mockGraph, int node, int distance[], int start)
{
	std::stack<int> stack;
	if (node == start)
	{
		stack.push(node);
		return stack;
	}
	for (int i = 0; i < mockGraph.size(); ++i)
	{
		if (i == node) continue;
		if (mockGraph[i][node] != 0 && distance[i] == distance[node] - 1)
		{
			stack.push(node);
			if (i != start)
			{
				std::stack<int> tmpStack;
				tmpStack = outputChain(mockGraph, i, distance, start);
				reverseStack(tmpStack);
				while (!tmpStack.empty())
				{
					stack.push(tmpStack.top());
					tmpStack.pop();
				}
			}
			else
			{
				stack.push(i);
			}
			break;
		}
	}

	return stack;
}

void outputReactionChains(
	const ofstream& ofs,
	const std::vector<std::vector<std::vector<int>>>& mockGraph,
	int input,
	const int output,
	std::vector<EffectNode> effectNodes
)
{
	std::queue<int> q;
	int distance[mockGraph.size()];
	for (auto& dist: distance) dist = -1;

	int chains[mockGraph.size()];
	for (auto& chain : chains) chain = -1;

	q.push(input);
	distance[input] = 0;
	while (!q.empty())
	{
		int v = q.front();
		if (distance[v] + 1 == 5)
		{
			break;
		}
		q.pop();
		for (int i = 0; i < mockGraph[v].size(); i++)
		{
			if (i == v) continue;
			if (!mockGraph[v][i].empty())
			{
				q.push(i);
				distance[i] = distance[v] + 1;
				chains[i] = v;
			}
		}
	}

//	std::sort(chains.begin(), chains.end());
	for (auto chain: chains)
	{
		if (chain >= 0)
		{
			std::cout << chain << " <== ";
		}
	}
	std::cout << std::endl;
}

std::vector<std::vector<std::vector<int>>>
createGraph(std::vector<string>& quantityList, std::vector<EffectNode>& effectNodes)
{
	std::vector<std::vector<std::vector<int>>> graph{};
	for (int i = 0; i < quantityList.size(); i++)
	{
		std::vector<std::vector<int>> nodeEdges{};
		for (int j = 0; j < quantityList.size(); j++)
		{
			std::vector<int> effects{};
			if (i == j)
			{
				nodeEdges.push_back(effects);
				continue;
			}

			for (int k = 0; k < effectNodes.size(); k++)
			{
				if (std::find(effectNodes[k].input.begin(), effectNodes[k].input.end(), i) != effectNodes[k].input.end()
					&& effectNodes[k].output == j)
				{
					effects.push_back(k);
				}
			}

			nodeEdges.push_back(effects);
		}
		graph.push_back(nodeEdges);
	}

	return graph;
}

std::vector<EffectNode> parseEffectsFile(ifstream& ifs)
{
	std::vector<EffectNode> effectNodes{};
	int i = 0;
	while (!ifs.eof())
	{
		std::string line;
		EffectNode node{};
		getline(ifs, line);
		int flag = 0;
		std::string inp{};
		for (char ch: line)
		{
			if (ch == ';')
			{
				if (flag > 0)
				{
					int num = std::stoi(inp);
					switch (flag)
					{
					case 1:
						node.input.push_back(num);
						break;
					case 2:
						node.output = num;
						break;
					default:
						break;
					}
					inp = {};
				}

				flag++;
				continue;
			}
			if (ch == ',')
			{
				int num = std::stoi(inp);
				switch (flag)
				{
				case 1:
					node.input.push_back(num);
					break;
				case 2:
					node.output = num;
					break;
				}
				inp = {};
				continue;
			}
			switch (flag)
			{
			case 0:
				node.name += ch;
				break;
			case 1:
			case 2:
				inp += ch;
				break;
			}
		}
		effectNodes.push_back(node);
		i++;
		ifs.peek();
	}

	return effectNodes;
}

std::vector<std::string> parsePhysicalQuantityFile(ifstream& ifs)
{
	std::vector<std::string> physicalQuantities{};
	int i = 0;
	while (!ifs.eof())
	{
		std::string line;
		std::string quantity{};
		getline(ifs, line);
		int flag = 0;
		std::string inp{};
		for (char ch: line)
		{
			if (flag == 1)
			{
				quantity += ch;
			}
			if (ch == ':')
			{
				flag = 1;
			}
		}
		physicalQuantities.push_back(quantity);
		i++;
		ifs.peek();
	}

	return physicalQuantities;
}

int main()
{
	setlocale(0, "");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	wcout << L"Введите название файла с физическими величинами: ";
	string inFileNamePower;
	cin >> inFileNamePower;
	ifstream ifsPower(inFileNamePower);
	if (!ifsPower)
	{
		std::wstring wsTmp(inFileNamePower.begin(), inFileNamePower.end());
		wcerr << L"Не получилось открыть файл: " << wsTmp << endl;
		exit(1);
	}

	std::vector<std::string> quantityList = parsePhysicalQuantityFile(ifsPower);
	cout << quantityList[0];

	std::wcout << L"Введите название входного файла с физическими величинами: ";
	string inFileNameEffects;
	cin >> inFileNameEffects;
	ifstream ifsEffects(inFileNameEffects);
	if (!ifsEffects)
	{
		std::wstring wsTmp(inFileNameEffects.begin(), inFileNameEffects.end());
		wcerr << TEXT("Не получилось открыть файл: ") << wsTmp << endl;
		exit(1);
	}

	std::vector<EffectNode> effectNodes = parseEffectsFile(ifsEffects);

	const std::vector<std::vector<std::vector<int>>> graph = createGraph(quantityList, effectNodes);

	int input = 18;
	int output = 10;

//	std::wcout << L"Введите input: ";
//	cin >> input;
//	std::wcout << L"Введите output: ";
//	cin >> output;

	wcout << L"Введите название выходного файла: ";
	string outFileName;
	cin >> outFileName;
	ofstream ofs;

	outputReactionChains(ofs, graph, input, output, effectNodes);

	ifsPower.close();
	ifsEffects.close();
	ofs.close();

	return 0;
}