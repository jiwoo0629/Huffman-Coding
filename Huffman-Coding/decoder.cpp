#include "decoder.h"

inline string readfile(const char* filename)
{
	ifstream fp;
	fp.open(filename, ios::binary);
	fp.seekg(0, ios::end);
	long filesize = (long)fp.tellg(); //���ϻ����� ���ϱ�
	fp.seekg(0, ios::beg);
	unsigned char* buffer = new unsigned char[filesize];
	fp.read((char*)buffer, filesize); //huffman_table.hbs ���� �б�
	
	//buffer�� 8bit���� �о�ͼ� string ���·� ����
	string huffman_file = "";
	for (int i = 0; i < filesize; i++)
		huffman_file += bitset<8>(buffer[i]).to_string();

	fp.close();
	return huffman_file;
}

inline vector<pair<pair<char, int>, string>> table_decoding(string huffman_table, vector<pair<pair<char, int>, string>> vvv)
{
	unsigned int at = 0; //���ڿ��� �б� ������ ��ġ�� ������ ����
	while (at < huffman_table.size())
	{
		//8bit ��ŭ �о�� char�� ����
		int num1 = 0;
		for (unsigned int i = at; i < at + 8; i++)
		{
			int n = huffman_table.at(i) - '0';
			num1 += (n * (int)pow(2, (7 - i + at)));
		}
		char c1 = (char)num1;
		at += 8;
		//���� 8bit ��ŭ �о�� int�� ����
		int num2 = 0;
		for (unsigned int i = at; i < at + 8; i++)
		{
			int n = huffman_table.at(i) - '0';
			num2 += (n * (int)pow(2, (7 - i + at)));
		}
		at += 8;
		//��� �о�� 8bit�� ��Ÿ���� ���� ���̸�ŭ �о�� string���� ����
		string s = "";
		for (unsigned int i = at; i < at + num2; i++)
			s += huffman_table.at(i);
		at += num2;

		vvv.push_back(make_pair(make_pair(c1, num2), s));
		int at8 = at + 8;
		//byte align�� ������ ������� �ʱ� ���� ����ó��
		if (at8 > huffman_table.size()) 
			break;
	}
	sort(vvv.begin(), vvv.end(), compare); //huffman code�� bit���� ���� �������� ����
	return vvv;
}

inline vector<int> getlen(vector<pair<pair<char, int>, string>> vvv)
{
	vector<int> len;
	len.push_back(vvv[0].first.second);
	int past = len[0];
	for (unsigned int i = 1; i < vvv.size(); i++)
	{
		if (vvv[i].first.second > past)
		{
			len.push_back(vvv[i].first.second);
			past = len.back();
		}
	}
	return len;
}

inline string code_decoding(string huffman_code, vector<pair<pair<char, int>, string>> vvv, int length)
{
	//1byte Ȯ���ؼ� ��ġ�ϴ� �� string�� �ִ��� Ȯ��
	//���� ��� decoding�� ��� ������ string�� �����ϰ� at�� bit ���̸�ŭ ����
	//���� ��� 2byte�� Ȯ���� �ݺ�
	string output = "";
	unsigned int at = 0;
	while (at < huffman_code.size())
	{
		string comstr = "";
		for (unsigned int i = at; i < at + 8; i++)
			comstr.append(1, huffman_code.at(i)); //���Ƿ� 1byte��ŭ �о��

		int count = 0;
		int exist = 0;
		while (vvv[count].first.second <= 8)
		{
			//huffman code�� bit length�� ���� ������ �ϳ��� �о�� ����
			string comstr2 = vvv[count].second;
			string comstrcp = ""; //comstr2�� bit length��ŭ comstr���� ������ comstrcp�� ����
			for (int i = 0; i < vvv[count].first.second; i++)
				comstrcp.append(1, comstr.at(i));
			if (comstrcp.compare(comstr2) == 0) //�� ���ڿ��� ���� ��, �ش� ���ڰ� �Էµ� ������ ����
			{
				output.append(1, vvv[count].first.first);
				at += vvv[count].first.second;
				exist = 1;
				break;
			}
			count++;
		}
		int at8 = at + 8;
		if (at8 >= huffman_code.size())
			break;

		if (exist == 0)
		{
			for (unsigned int i = at + 8; i < at + 16; i++)
				comstr.append(1, huffman_code.at(i));
			while (vvv[count].first.second <= 16)
			{
				string comstr2 = vvv[count].second;
				string comstrcp = "";
				for (int i = 0; i < vvv[count].first.second; i++)
					comstrcp.append(1, comstr.at(i));
				if (comstrcp.compare(comstr2) == 0) //�� ���ڿ��� ���� ��, �ش� ���ڰ� �Էµ� ������ ����
				{
					output.append(1, vvv[count].first.first);
					at += vvv[count].first.second;
					break;
				}
				count++;
				if (count == vvv.size())
					break;
			}
			at8 = at + 8;
			if (at8 >= huffman_code.size())
				break;
		}
	}
	return output;
}

inline bool compare(pair<pair<char, int>, string> a, pair<pair<char, int>, string> b)
{
	return a.first.second < b.first.second;
}