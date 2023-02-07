#include "encoder.h"

inline vector<pair<char, int>> makeV(vector<char> store)
{
	vector<pair<char, int>> vv;

	for (unsigned int i = 0; i < store.size(); i++)
	{
		char getc = store[i]; //������ ���ۺ��� �� ���� Ȯ��
		if (i == 0) //�ʱⰪ ����
		{
			vv.push_back(make_pair(getc, 1));
			continue;
		}
		int exist = 0; //���ڰ� �̹� carr�� �����ϴ��� Ȯ��
		for (char j = 0; j < 127; j++) //ASCII �� ��
		{
			if (store[i] == j)
			{
				for (unsigned int k = 0; k < vv.size(); k++)
				{
					//���ڰ� ��ġ�� ��� �ش� ������ ���� �߰�
					if (vv[k].first == store[i])
					{
						vv[k].second++;
						exist = 1;
						continue;
					}
				}
			}
		}
		if (exist == 0) //�������� ���� ��� carr�� �߰�
			vv.push_back(make_pair(store[i], 1));
	}
	return vv;
}

inline const char* huffman_table(vector <pair< pair<char, int>, string >> vvv)
{
	ofstream table;
	table.open("huffman_table.hbs", ios::binary); //huffman_table.hbs�� binary ���� ���·� ����

	string huffman_table = "";
	for (unsigned int i = 0; i < vvv.size(); i++)
	{
		//vvv vector�� char, int�� ���� 8 bitset���� ������ ���ڿ��� ��ȯ�� huffman_table string�� ����
		huffman_table += bitset<8>(vvv[i].first.first).to_string();
		int length = (vvv[i].second).size();
		huffman_table += bitset<8>(length).to_string();
		huffman_table += vvv[i].second;
	}
	//byte align�� ���� ���ڶ�� bit��ŭ ä����
	int stuffingbit = 8 - (huffman_table.length() % 8);
	for (int i = 0; i < stuffingbit; i++)
		huffman_table += "0";

	//huffman_table string�� �� ���ڸ� �� bit�� ����
	//num�� 1 byte ���̸�, �ش� ���� byte Ÿ������ huffman_table.hbs ���Ͽ� write
	for (unsigned int i = 0; i < (huffman_table.size() / 8); i++)
	{
		int n = 0;
		for (int j = 0; j < 8; j++)
		{
			int at = 8 * i + j;
			char c = huffman_table.at(at);
			int num = c - '0';
			n += (num * (int)pow(2, (7 - j)));
		}
		table.write((char*)&n, 1);
	}
	table.close();
	const char* tablename = "huffman_table.hbs";
	return tablename;
}

inline const char* huffman_code(vector<char> store, vector <pair< pair<char, int>, string >> vvv)
{
	ofstream code;
	code.open("huffman_code.hbs", ios::binary); //huffman_code.hbs�� binary ���� ���·� ����

	string huffman_code = "";
	for (unsigned int i = 0; i < store.size(); i++)
	{
		//store vector�� �� �ڸ� ���ڿ� vvv vector�� char�� ���Ͽ� ��ġ�ϸ�, �� ���� huffman code ���� huffman_code string�� ������
		for (auto& it : vvv)
		{
			if (store[i] == it.first.first)
			{
				huffman_code += it.second;
				continue;
			}
		}
	}
	//byte align�� ���� ���ڶ�� bit��ŭ ä����
	int stuffingbit = 8 - (huffman_code.length() % 8);
	for (int i = 0; i < stuffingbit; i++)
		huffman_code += "0";

	//huffman_code string�� �� ���ڸ� �� bit�� ����
	//num�� 1 byte ���̸�, �ش� ���� byte Ÿ������ huffman_code.hbs ���Ͽ� write
	for (unsigned int i = 0; i < (huffman_code.size() / 8); i++)
	{
		int n = 0;
		for (int j = 0; j < 8; j++)
		{
			int at = 8 * i + j;
			char c = huffman_code.at(at);
			int num = c - '0';
			n += (num * (int)pow(2, (7 - j)));
		}
		code.write((char*)&n, 1);
	}
	code.close();
	const char* codename = "huffman_code.hbs";
	return codename;
}

inline bool compVV(pair<char, int> a, pair<char, int> b)
{
	return a.second > b.second;
}

inline bool compVVV(pair<pair<char, int>, string> a, pair<pair<char, int>, string> b)
{
	return a.first.second < b.first.second;
}

inline void compression(const char* tablename, const char* codename, const char* inputname)
{
	double tablefilesize = filesize(tablename); //huffman_table.hbs file size
	double codefilesize = filesize(codename); //huffman_code.hbs file size
	double inputfilesize = filesize(inputname); //input_data.txt file size

	double compression = (1 - ((tablefilesize + codefilesize) / inputfilesize)) * 100;

	cout << "Input size : " << inputfilesize << ", huffman table bs size : " << tablefilesize << ", data bs size : " << codefilesize << endl;
	cout << "�����(Compression rate) = " << compression << "%" << endl;
}

inline double filesize(const char* filename)
{
	ifstream fp;
	fp.open(filename);
	fp.seekg(0, ios::end);
	double filesize = (double)fp.tellg(); //filesize ���ϴ� �Լ�
	return filesize;
}