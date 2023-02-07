#include "decoder.h"

inline string readfile(const char* filename)
{
	ifstream fp;
	fp.open(filename, ios::binary);
	fp.seekg(0, ios::end);
	long filesize = (long)fp.tellg(); //파일사이즈 구하기
	fp.seekg(0, ios::beg);
	unsigned char* buffer = new unsigned char[filesize];
	fp.read((char*)buffer, filesize); //huffman_table.hbs 파일 읽기

	//buffer의 8bit마다 읽어와서 string 형태로 저장
	string huffman_file = "";
	for (int i = 0; i < filesize; i++)
		huffman_file += bitset<8>(buffer[i]).to_string();

	fp.close();
	return huffman_file;
}

inline vector<pair<pair<char, int>, string>> table_decoding(string huffman_table, vector<pair<pair<char, int>, string>> vvv)
{
	unsigned int at = 0; //문자열을 읽기 시작할 위치를 저장할 변수
	while (at < huffman_table.size())
	{
		//8bit 만큼 읽어와 char로 저장
		int num1 = 0;
		for (unsigned int i = at; i < at + 8; i++)
		{
			int n = huffman_table.at(i) - '0';
			num1 += (n * (int)pow(2, (7 - i + at)));
		}
		char c1 = (char)num1;
		at += 8;
		//다음 8bit 만큼 읽어와 int로 저장
		int num2 = 0;
		for (unsigned int i = at; i < at + 8; i++)
		{
			int n = huffman_table.at(i) - '0';
			num2 += (n * (int)pow(2, (7 - i + at)));
		}
		at += 8;
		//방금 읽어온 8bit가 나타내는 값의 길이만큼 읽어와 string으로 저장
		string s = "";
		for (unsigned int i = at; i < at + num2; i++)
			s += huffman_table.at(i);
		at += num2;

		vvv.push_back(make_pair(make_pair(c1, num2), s));
		int at8 = at + 8;
		//byte align을 포함해 계산하지 않기 위해 예외처리
		if (at8 > huffman_table.size())
			break;
	}
	sort(vvv.begin(), vvv.end(), compare); //huffman code의 bit수에 따라 오름차순 정렬
	return vvv;
}

inline vector<pair<char, vector<pair<pair<char, int>, string>>>> CAtable_decoding(string CAhuffman_table, vector<pair<char, vector<pair<pair<char, int>, string>>>> vvCAD)
{
	unsigned int at = 0; //문자열을 읽기 시작할 위치를 저장할 변수
	while (at < CAhuffman_table.size())
	{
		//8bit 만큼 읽어와 char로 저장
		int num1 = 0;
		for (unsigned int i = at; i < at + 8; i++)
		{
			int n = CAhuffman_table.at(i) - '0';
			num1 += (n * (int)pow(2, (7 - i + at)));
		}
		char c1 = (char)num1;
		at += 8;
		//다음 8bit 만큼 읽어와 char로 저장
		int num2 = 0;
		for (unsigned int i = at; i < at + 8; i++)
		{
			int n = CAhuffman_table.at(i) - '0';
			num2 += (n * (int)pow(2, (7 - i + at)));
		}
		char c2 = (char)num2;
		at += 8;
		//다음 8bit 만큼 읽어와 int로 저장
		int num3 = 0;
		for (unsigned int i = at; i < at + 8; i++)
		{
			int n = CAhuffman_table.at(i) - '0';
			num3 += (n * (int)pow(2, (7 - i + at)));
		}
		at += 8;
		//방금 읽어온 8bit가 나타내는 값의 길이만큼 읽어와 string으로 저장
		string s = "";
		for (unsigned int i = at; i < at + num3; i++)
			s += CAhuffman_table.at(i);
		at += num3;

		int exist = 0;
		for (auto& it : vvCAD)
		{
			if (it.first == c1) //vvCAD에 prev 값이 c1인 vector가 이미 존재할 경우
			{
				it.second.push_back(make_pair(make_pair(c2, num3), s));
				exist = 1;
				continue;
			}
		}
		if (exist == 0) //존재하지 않을 경우
		{
			vector < pair<pair<char, int>, string>> vCA;
			vCA.push_back(make_pair(make_pair(c2, num3), s));
			vvCAD.push_back(make_pair(c1, vCA));
		}
		
		//byte align을 포함해 계산하지 않기 위해 예외처리
		int at8 = at + 8;
		if (at8 > CAhuffman_table.size())
			break;
	}
	for (int i = 0; i < vvCAD.size(); i++) //각 문자별로 huffman code의 bit 수에 따라 오름차순 정렬
		sort(vvCAD[i].second.begin(), vvCAD[i].second.end(), compare);
	
	return vvCAD;
}

inline string code_decoding(string CAhuffman_code, vector<pair<pair<char, int>, string>> vvv, vector<pair<char, vector<pair<pair<char, int>, string>>>> vvCAD)
{
	//1byte 확인해서 일치하는 값 string이 있는지 확인
	//있을 경우 decoding한 결과 저장할 string에 저장하고 at을 bit 길이만큼 더함
	//없을 경우 2byte로 확장해 반복
	string output = "";
	unsigned int at = 0;
	string comstr = "";
	for (unsigned int i = at; i < at + 8; i++)
		comstr.append(1, CAhuffman_code.at(i));
	int c = 0, e = 0;
	char prev = 0; //이전 문자
	while (vvv[c].first.second <= 8)
	{
		//huffman code의 bit length가 작은 값부터 하나씩 읽어와 비교함
		string comstr2 = vvv[c].second;
		string comstrcp = ""; //comstr2의 bit length만큼 comstr에서 복사해 comstrcp에 저장
		for (int i = 0; i < vvv[c].first.second; i++)
			comstrcp.append(1, comstr.at(i));
		if (comstrcp.compare(comstr2) == 0) //두 문자열이 같을 때, 해당 문자가 입력된 것으로 간주
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
			if (comstrcp.compare(comstr2) == 0) //두 문자열이 같을 때, 해당 문자가 입력된 것으로 간주
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

	//prev 기준으로 append 하게 설계
	while (at < CAhuffman_code.size())
	{
		string comstrx = "";
		if (at + 8 < CAhuffman_code.size())
		{
			for (unsigned int i = at; i < at + 8; i++)
				comstrx.append(1, CAhuffman_code.at(i)); //임의로 1byte만큼 읽어옴
		}
		else
		{
			for (unsigned int i = at; i < CAhuffman_code.size(); i++)
				comstrx.append(1, CAhuffman_code.at(i)); //임의로 1byte만큼 읽어옴
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
			//CAhuffman code의 bit length가 작은 값부터 하나씩 읽어와 비교함
			string comstr2 = vvCAD[index].second[count].second;
			string comstrcp = ""; //comstr2의 bit length만큼 comstr에서 복사해 comstrcp에 저장
			for (int i = 0; i < vvCAD[index].second[count].first.second; i++)
				comstrcp.append(1, comstrx.at(i));
			if (comstrcp.compare(comstr2) == 0) //두 문자열이 같을 때, 해당 문자가 입력된 것으로 간주
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
					comstrx.append(1, CAhuffman_code.at(i)); //임의로 1byte만큼 읽어옴
			}
			else
			{
				for (unsigned int i = at + 8; i < CAhuffman_code.size(); i++)
					comstrx.append(1, CAhuffman_code.at(i)); //임의로 1byte만큼 읽어옴
			}
			while (vvCAD[index].second[count].first.second <= 16)
			{
				//CAhuffman code의 bit length가 작은 값부터 하나씩 읽어와 비교함
				string comstr2 = vvCAD[index].second[count].second;
				string comstrcp = ""; //comstr2의 bit length만큼 comstr에서 복사해 comstrcp에 저장
				for (int i = 0; i < vvCAD[index].second[count].first.second; i++)
					comstrcp.append(1, comstrx.at(i));
				if (comstrcp.compare(comstr2) == 0) //두 문자열이 같을 때, 해당 문자가 입력된 것으로 간주
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