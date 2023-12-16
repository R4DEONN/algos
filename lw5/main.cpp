//{
//<Условие задачи:
// 21. Реализовать алгоритм поиска кратчайших путей Дейкстры на
//основе бинарной кучи и проиллюстрировать  по  шагам  этапы  его
//выполнения. Максимальная  размерность графа:  50000  вершин  и
//500000 дуг. Счет не более 2 сек.  Функции  для  работы с  кучей
//написать самостоятельно (12).
//>
//<Автор: Борисов Кирилл Александрович ПС-22 />
//<Среда выполнения: Jetbrains CLion Runtime version: 17.0.8+7-b1000.8 amd64 />
//<Источник информации: />
//}

#include <windows.h>
#include <locale>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "BinaryHeap.h"
#include "bprinter/table_printer.h"
using namespace bprinter;

using namespace std;

constexpr const int INF = INT_MAX;

vector<vector<pair<int, int>>> readGraphFromFile(ifstream& ifs)
{
	int vertexCount, edgeCount;
	ifs >> vertexCount >> edgeCount;

	vector<vector<pair<int, int>>> graph(vertexCount);
	for (int i = 0; i < edgeCount; i++)
	{
		int from, to, weight;
		ifs >> from >> to >> weight;

		graph[from].emplace_back(to, weight);
	}

	return graph;
}

int main()
{
	setlocale(0, "");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	wcout << L"Введите название входного файла: ";
	string inFileName;
	cin >> inFileName;
	ifstream ifs(inFileName);
	if (!ifs)
	{
		std::wstring wsTmp(inFileName.begin(), inFileName.end());
		wcerr << L"Не получилось открыть файл: " << wsTmp << std::endl;
		exit(1);
	}

	wcout << L"Введите название выходного файла: ";
	string outFileName;
	cin >> outFileName;
	ofstream ofs(outFileName);

	vector<vector<pair<int, int>>> graph = readGraphFromFile(ifs);

	int start;
	ifs >> start;

	vector<int> dist(graph.size(), INF);
	dist[start] = 0;

	TablePrinter tp(&ofs);
	tp.AddColumn("Num", 5);
	for (int i = 0; i < graph.size(); i++)
	{
		tp.AddColumn(to_string(i), 10);
	}

	BinaryHeap heap;
	heap.push({dist[start], start});
	tp.PrintHeader();

	int num = 0;
	while (!heap.empty())
	{
		tp << num;
		num++;
		for (auto d : dist)
		{
			if (d != INF)
			{
				tp << d;
			}
			else
			{
				tp << "X";
			}
		}
		tp << bprinter::endl();
		auto [nearestDist, nearest] = heap.top();
		heap.pop();

		for (auto& [to, weight] : graph[nearest])
		{
			if (dist[to] > dist[nearest] + weight)
			{
				dist[to] = dist[nearest] + weight;
				heap.push({ dist[to], to });
			}
		}
	}
	tp.PrintFooter();
}