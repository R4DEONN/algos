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

using namespace std;

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



	ifs.close();
	ofs.close();

	return 0;
}