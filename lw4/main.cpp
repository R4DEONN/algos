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
#include <locale>
#include <codecvt>
#include <cstdlib>

using namespace std;

struct EffectNode
{
	std::string name{};
	std::vector<int> input{};
	int output{};
};

void searchWays(
	std::vector<std::vector<std::vector<int>>>& graph,
	int input,
	int output,
	std::vector<std::vector<int>>& ways,
	std::vector<int>& way,
	std::vector<EffectNode>& effectNodes
)
{
	way.push_back(input);
	if (input == output)
	{
		ways.push_back(way);
		way.pop_back();
		return;
	}

	if (way.size() >= 4)
	{
		way.pop_back();
		return;
	}

	for (int i = 0; i < graph[input].size(); i++)
	{
		if (graph[input][i].empty() || std::find(way.begin(), way.end(), i) != way.end())
		{
			continue;
		}
		searchWays(graph, i, output, ways, way, effectNodes);
	}
	way.pop_back();
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
				if (std::find(effectNodes[k].input.begin(), effectNodes[k].input.end(), i) !=
					effectNodes[k].input.end()
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
						node.input.push_back(num - 1);
						break;
					case 2:
						node.output = num - 1;
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
					node.input.push_back(num - 1);
					break;
				case 2:
					node.output = num - 1;
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

void outputWays(std::vector<std::vector<int>>& ways, wofstream& wofs, std::vector<std::string>& quantityList, std::vector<EffectNode> effectNodes)
{
	std::sort(ways.begin(), ways.end(), [](auto &way1, auto &way2){
		return way1.size() < way2.size();
	});
	for (auto& way : ways)
	{
		auto isFirst = true;
		for (auto quantity : way)
		{
			if (!isFirst)
			{
				wofs << L" ==> ";
			}
			isFirst = false;
			std::wstring ws(quantityList[quantity + 1].size(), L' ');
			ws.resize(std::mbstowcs(&ws[0], quantityList[quantity + 1].c_str(), quantityList[quantity + 1].size()));
			wofs << ws;
		}
		wofs << endl;
		for (int i = 0; i < way.size() - 1; i++)
		{
			wofs << L"Звено ";
			std::wstring ws1(quantityList[way[i] + 1].size(), L' ');
			ws1.resize(std::mbstowcs(&ws1[0], quantityList[way[i] + 1].c_str(), quantityList[way[i] + 1].size()));
			std::wstring ws2(quantityList[way[i + 1] + 1].size(), L' ');
			ws2.resize(std::mbstowcs(&ws2[0], quantityList[way[i + 1] + 1].c_str(), quantityList[way[i + 1] + 1].size()));
			wofs << to_wstring(i + 1) << ": " << ws1 << " ==> " << ws2 << endl;
			for (auto& effect : effectNodes)
			{
				if (find(effect.input.begin(), effect.input.end(), way[i]) != effect.input.end()
					&& effect.output == way[i + 1])
				{
					std::wstring ws3(effect.name.size(), L' ');
					ws3.resize(std::mbstowcs(&ws3[0], effect.name.c_str(), effect.name.size()));
					wofs << ws3 << endl;
				}
			}
		}
		wofs << endl;
	}
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

	std::wcout << L"Введите название входного файла с физическими величинами: ";
	string inFileNameEffects;
	cin >> inFileNameEffects;
	ifstream ifsEffects(inFileNameEffects);
	if (!ifsEffects)
	{
		std::wstring wsTmp(inFileNameEffects.begin(), inFileNameEffects.end());
		wcerr << L"Не получилось открыть файл: " << wsTmp << endl;
		exit(1);
	}

	std::vector<EffectNode> effectNodes = parseEffectsFile(ifsEffects);

	std::vector<std::vector<std::vector<int>>> graph = createGraph(quantityList, effectNodes);

	int input = 0;
	int output = 0;

	std::wcout << L"Введите input: ";
	cin >> input;
	std::wcout << L"Введите output: ";
	cin >> output;

	wcout << L"Введите название выходного файла: ";
	string outFileName;
	cin >> outFileName;
	wofstream ofs(outFileName);
	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	ofs.imbue(loc);

	std::vector<std::vector<int>> ways{};
	std::vector<int> way{};
	searchWays(graph, input, output, ways, way, effectNodes);
	outputWays(ways, ofs, quantityList, effectNodes);

	ifsPower.close();
	ifsEffects.close();
	ofs.close();

	return 0;
}