//{
//<Условие задачи:
//27. Имеется  И-ИЛИ   дерево,   соответствующее   некоторому
//множеству  конструкций.  Требуется выдать на экран в наглядном
//виде все элементы дерева (14).
//>
//<Автор: Борисов Кирилл Александрович ПС-22>
//<Среда выполнения: Jetbrains CLion Runtime version: 17.0.8+7-b1000.8 amd64>
//<Источник информации: https://acm.bsu.by/wiki/Программная_реализация_бинарных_поисковых_деревьев#C.2B.2B>
//}
#include <windows.h>
#include <iostream>
#include <fstream>
#include <ios>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include "andOrTree.h"

using namespace std;

void outputTrees(ifstream& ifs, ofstream& ofs)
{
	string str;
	auto tree = new AndOrTree();
	tree->ReadTreeFromStream(ifs);
	tree->OutputTreeToStream(ofs);

	int number = 1;
	stack<AndOrTreeNode*> orNodesOutputLeaves;
	stack<AndOrTreeNode*> nodesToReset;
	bool isOutput = false;
	while (!tree->isAllTreesOutput() && !isOutput)
	{
		ofs << endl << endl;
		ofs << "Number " << number << endl;
		isOutput = tree->OutputTreesToStream(ofs, orNodesOutputLeaves);
		number++;
	}
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
		wcerr << L"Не получилось открыть файл: " << wsTmp << endl;
		exit(1);
	}

	wcout << L"Введите название выходного файла: ";
	string outFileName;
	cin >> outFileName;
	ofstream ofs(outFileName, ios::out);

	outputTrees(ifs, ofs);

	ifs.close();
	ofs.close();

	return 0;
}