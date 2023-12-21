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
//<Источник информации:
// 1) https://github.com/dattanchu/bprinter/wiki
// 2) https://www.youtube.com/watch?v=J-7MzbEtTR0&t=355s
// 3) https://ru.wikipedia.org/wiki/Двоичная_куча
// 4) http://mech.math.msu.su/~nap/2/2_18.html
// />
//}

#include <windows.h>
#include <locale>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "BinaryHeap.h"
//#include "bprinter/table_printer.h"
//using namespace bprinter;

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

		graph[from - 1].emplace_back(to - 1, weight);
	}

	return graph;
}

int main()
{
	setlocale(0, "");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	auto startTime = clock();

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
	int end;
	ifs >> start >> end;
	start--;
	end--;

	vector<int> dist(graph.size(), INF);
	dist[start] = 0;
	vector<int> from(graph.size(), -1);

	BinaryHeap heap;
	heap.push({dist[start], start});

	while (!heap.empty())
	{
		auto [nearestDist, nearest] = heap.top();
		heap.pop();

		for (auto& [to, weight] : graph[nearest])
		{
			if (dist[to] > dist[nearest] + weight)
			{
				dist[to] = dist[nearest] + weight;
				from[to] = nearest;
				heap.push({ dist[to], to });
			}
		}
	}
	ofs << dist[end] << endl;
	vector<int> path;
	for (int v = end; v != -1; v = from[v])
	{
		path.push_back(v);
	}
	std::reverse(path.begin(), path.end());
	for (int p : path)
	{
		ofs << p + 1 << " ";
	}
	ofs << endl;
	unsigned int endTime = clock(); // конечное время
	unsigned int search_time = endTime - startTime; // искомое время
	cout << search_time / 1000;
}