//
//{
//<������� ������:
// 17. ��������� ������� ��������  ������.  �������  �  ������
// 1-31,243-255 �� ������,  �.�. ��� ����� ������ ���� ���������.
// �������� ��������� ��������-���������� ����������  �����. ����
// �� ������ ���� ������� �� ����������� ������  �������  �������
// ������ �� ����� ������. �������� � ������������� �����  ������
// ����� ���������� ������ (8).
//>
//<�����: ������� ������ ������������� ��-22>
//<����� ����������: Jetbrains CLion Runtime version: 17.0.8+7-b1000.8 amd64>
//<�������� ����������: >
//
// <����� ���������: "�������� ���������" (��� �������� "encrypt"|��� ���������� "decrypt")
//}

#include <windows.h>
#include <iostream>
#include <fstream>
#include <ios>
#include <string>
using namespace std;

const char* ENCRYPT = "encrypt";

const char* DECRYPT = "decrypt";

bool isServiceSymbol(unsigned char symbol)
{
	return (symbol > 1 && symbol < 32)
		   || (symbol > 243 && symbol < 255);
}

void encryptAuthor(ifstream &ifs, ofstream &ofs, string key, bool enc)
{
	int i = 0;
	string line;
	cout << line << endl;
	while (!ifs.eof())
	{
		getline(ifs, line);
		for (unsigned char ch : line)
		{
			if (isServiceSymbol(ch) || isServiceSymbol(ch + key[i]) || isServiceSymbol(ch - key[i]))
			{
				ofs << ch;
				continue;
			}
			char enryptedCh;
			if (enc)
			{
				if (!isServiceSymbol(ch + key[i]))
				{
					enryptedCh = ch + key[i];
				}
				else
				{
					enryptedCh = ch - key[i];
				}
			}
			else
			{
				if (!isServiceSymbol(ch - key[i]))
				{
					enryptedCh = ch - key[i];
				}
				else
				{
					enryptedCh = ch + key[i];
				}
			}

			if (i == key.length() - 1)
			{
				i = 0;
			}
			else
			{
				i++;
			}
			ofs << enryptedCh;
		}
		ifs.peek();
		if (!ifs.eof())
		{
			ofs << endl;
		}
	}
}

int main(int argc, char** argv)
{
	setlocale(0, "");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	wcout << TEXT("������� �������� �������� �����: ");
	string inFileName;
	cin >> inFileName;
	ifstream inpFile(inFileName);
	if (!inpFile)
	{
		std::wstring wsTmp(inFileName.begin(), inFileName.end());
		wcerr << TEXT("�� ���������� ������� ����: ") << wsTmp << endl;
		exit(1);
	}

	wcout << TEXT("������� �������� ��������� �����: ");
	string outFileName;
	cin >> outFileName;
	ofstream outFile(outFileName, ios::out);

	string key;
	wcout << TEXT("�������e ����: ");
	cin >> key;

	bool enc;
	if (strcmp(argv[1], ENCRYPT) == 0)
	{
		enc = true;
	}
	else if (strcmp(argv[1], DECRYPT) == 0)
	{
		enc = false;
	}

	encryptAuthor(inpFile, outFile, key, enc);

	inpFile.close();
	outFile.close();

	return 0;
}