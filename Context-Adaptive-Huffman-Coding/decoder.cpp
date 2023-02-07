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

inline vector<pair<char, vector<pair<pair<char, int>, string>>>> CAtable_decoding(string CAhuffman_table, vector<pair<char, vector<pair<pair<char, int>, string>>>> vvCAD)
{
	unsigned int at = 0; //���ڿ��� �б� ������ ��ġ�� ������ ����
	while (at < CAhuffman_table.size())
	{
		//8bit ��ŭ �о�� char�� ����
		int num1 = 0;
		for (unsigned int i = at; i < at + 8; i++)
		{
			int n = CAhuffman_table.at(i) - '0';
			num1 += (n * (int)pow(2, (7 - i + at)));
		}
		char c1 = (char)num1;
		at += 8;
		//���� 8bit ��ŭ �о�� char�� ����
		int num2 = 0;
		for (unsigned int i = at; i < at + 8; i++)
		{
			int n = CAhuffman_table.at(i) - '0';
			num2 += (n * (int)pow(2, (7 - i + at)));
		}
		char c2 = (char)num2;
		at += 8;
		//���� 8bit ��ŭ �о�� int�� ����
		int num3 = 0;
		for (unsigned int i = at; i < at + 8; i++)
		{
			int n = CAhuffman_table.at(i) - '0';
			num3 += (n * (int)pow(2, (7 - i + at)));
		}
		at += 8;
		//��� �о�� 8bit�� ��Ÿ���� ���� ���̸�ŭ �о�� string���� ����
		string s = "";
		for (unsigned int i = at; i < at + num3; i++)
			s += CAhuffman_table.at(i);
		at += num3;

		int exist = 0;
		for (auto& it : vvCAD)
		{
			if (it.first == c1) //vvCAD�� prev ���� c1�� vector�� �̹� ������ ���
			{
				it.second.push_back(make_pair(make_pair(c2, num3), s));
				exist = 1;
				continue;
			}
		}
		if (exist == 0) //�������� ���� ���
		{
			vector < pair<pair<char, int>, string>> vCA;
			vCA.push_back(make_pair(make_pair(c2, num3), s));
			vvCAD.push_back(make_pair(c1, vCA));
		}
		
		//byte align�� ������ ������� �ʱ� ���� ����ó��
		int at8 = at + 8;
		if (at8 > CAhuffman_table.size())
			break;
	}
	for (int i = 0; i < vvCAD.size(); i++) //�� ���ں��� huffman code�� bit ���� ���� �������� ����
		sort(vvCAD[i].second.begin(), vvCAD[i].second.end(), compare);
	
	return vvCAD;
}

inline string code_decoding(string CAhuffman_code, vector<pair<pair<char, int>, string>> vvv, vector<pair<char, vector<pair<pair<char, int>, string>>>> vvCAD)
{
	//1byte Ȯ���ؼ� ��ġ�ϴ� �� string�� �ִ��� Ȯ��
	//���� ��� decoding�� ��� ������ string�� �����ϰ� at�� bit ���̸�ŭ ����
	//���� ��� 2byte�� Ȯ���� �ݺ�
	string output = "";
	unsigned int at = 0;
	string comstr = "";
	for (unsigned int i = at; i < at + 8; i++)
		comstr.append(1, CAhuffman_code.at(i));
	int c = 0, e = 0;
	char prev = 0; //���� ����
	while (vvv[c].first.second <= 8)
	{
		//huffman code�� bit length�� ���� ������ �ϳ��� �о�� ����
		string comstr2 = vvv[c].second;
		string comstrcp = ""; //comstr2�� bit length��ŭ comstr���� ������ comstrcp�� ����
		for (int i = 0; i < vvv[c].first.second; i++)
			comstrcp.append(1, comstr.at(i));
		if (comstrcp.compare(comstr2) == 0) //�� ���ڿ��� ���� ��, �ش� ���ڰ� �Էµ� ������ ����
		{
			output.append(1, vvv[c].first.first);
			prev = vvv[c].first.first;
			at += vvv[c].first.second;
			e = 1;
			break;
		}
		c++;
	}
	if (e == 0)
	{
		for (unsigned int i = at + 8; i < at + 16; i++)
			comstr.append(1, CAhuffman_code.at(i));
		while (vvv[c].first.second <= 16)
		{
			string comstr2 = vvv[c].second;
			string comstrcp = "";
			for (int i = 0; i < vvv[c].first.second; i++)
				comstrcp.append(1, comstr.at(i));
			if (comstrcp.compare(comstr2) == 0) //�� ���ڿ��� ���� ��, �ش� ���ڰ� �Էµ� ������ ����
			{
				output.append(1, vvv[c].first.first);
				prev = vvv[c].first.first;
				at += vvv[c].first.second;
				break;
			}
			c++;
			if (c == vvv.size())
				break;
		}
	}

	//prev �������� append �ϰ� ����
	while (at < CAhuffman_code.size())
	{
		string comstrx = "";
		if (at + 8 < CAhuffman_code.size())
		{
			for (unsigned int i = at; i < at + 8; i++)
				comstrx.append(1, CAhuffman_code.at(i)); //���Ƿ� 1byte��ŭ �о��
		}
		else
		{
			for (unsigned int i = at; i < CAhuffman_code.size(); i++)
				comstrx.append(1, CAhuffman_code.at(i)); //���Ƿ� 1byte��ŭ �о��
		}
		int index = 0;
		for (int i = 0; i < vvCAD.size(); i++)
		{
			if (vvCAD[i].first == prev)
			{
				index = i;
				break;
			}
		}

		int count = 0, exist = 0;
		while (vvCAD[index].second[count].first.second <= 8)
		{
			//CAhuffman code�� bit length�� ���� ������ �ϳ��� �о�� ����
			string comstr2 = vvCAD[index].second[count].second;
			string comstrcp = ""; //comstr2�� bit length��ŭ comstr���� ������ comstrcp�� ����
			for (int i = 0; i < vvCAD[index].second[count].first.second; i++)
				comstrcp.append(1, comstrx.at(i));
			if (comstrcp.compare(comstr2) == 0) //�� ���ڿ��� ���� ��, �ش� ���ڰ� �Էµ� ������ ����
			{
				output.append(1, vvCAD[index].second[count].first.first);
				prev = vvCAD[index].second[count].first.first;
				at += vvCAD[index].second[count].first.second;
				exist = 1;
				break;
			}
			count++;
		}
		int at8 = at + 8;
		if (at8 >= CAhuffman_code.size())
			break;

		if (exist == 0)
		{
			if (at + 16 < CAhuffman_code.size())
			{
				for (unsigned int i = at + 8; i < at + 16; i++)
					comstrx.append(1, CAhuffman_code.at(i)); //���Ƿ� 1byte��ŭ �о��
			}
			else
			{
				for (unsigned int i = at + 8; i < CAhuffman_code.size(); i++)
					comstrx.append(1, CAhuffman_code.at(i)); //���Ƿ� 1byte��ŭ �о��
			}
			while (vvCAD[index].second[count].first.second <= 16)
			{
				//CAhuffman code�� bit length�� ���� ������ �ϳ��� �о�� ����
				string comstr2 = vvCAD[index].second[count].second;
				string comstrcp = ""; //comstr2�� bit length��ŭ comstr���� ������ comstrcp�� ����
				for (int i = 0; i < vvCAD[index].second[count].first.second; i++)
					comstrcp.append(1, comstrx.at(i));
				if (comstrcp.compare(comstr2) == 0) //�� ���ڿ��� ���� ��, �ش� ���ڰ� �Էµ� ������ ����
				{
					output.append(1, vvCAD[index].second[count].first.first);
					prev = vvCAD[index].second[count].first.first;
					at += vvCAD[index].second[count].first.second;
					break;
				}
				count++;
				if (count == vvCAD[index].second.size())
					break;
			}
			at8 = at + 8;
			if (at8 >= CAhuffman_code.size())
				break;
		}
	}
	return output;
}

inline bool compare(pair<pair<char, int>, string> a, pair<pair<char, int>, string> b)
{
	return a.first.second < b.first.second;
}