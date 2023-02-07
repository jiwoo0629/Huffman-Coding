#include "encoder.h"

inline vector<pair<char, int>> makeV(vector<char> store)
{
	vector<pair<char, int>> vv;

	for (unsigned int i = 0; i < store.size(); i++)
	{
		char getc = store[i]; //파일의 시작부터 한 문자 확인
		if (i == 0) //초기값 설정
		{
			vv.push_back(make_pair(getc, 1));
			continue;
		}
		int exist = 0; //문자가 이미 carr에 존재하는지 확인
		for (char j = 0; j < 127; j++) //ASCII 값 비교
		{
			if (store[i] == j)
			{
				for (unsigned int k = 0; k < vv.size(); k++)
				{
					//문자가 일치할 경우 해당 문자의 개수 추가
					if (vv[k].first == store[i])
					{
						vv[k].second++;
						exist = 1;
						continue;
					}
				}
			}
		}
		if (exist == 0) //존재하지 않을 경우 carr에 추가
			vv.push_back(make_pair(store[i], 1));
	}
	return vv;
}

inline const char* huffman_table(vector <pair< pair<char, int>, string >> vvv)
{
	ofstream table;
	table.open("huffman_table.hbs", ios::binary); //huffman_table.hbs를 binary 파일 형태로 오픈

	string huffman_table = "";
	for (unsigned int i = 0; i < vvv.size(); i++)
	{
		//vvv vector의 char, int를 각각 8 bitset으로 생각해 문자열로 변환해 huffman_table string에 더함
		huffman_table += bitset<8>(vvv[i].first.first).to_string();
		int length = (vvv[i].second).size();
		huffman_table += bitset<8>(length).to_string();
		huffman_table += vvv[i].second;
	}
	//byte align을 위해 모자라는 bit만큼 채워줌
	int stuffingbit = 8 - (huffman_table.length() % 8);
	for (int i = 0; i < stuffingbit; i++)
		huffman_table += "0";

	//huffman_table string의 한 문자를 한 bit로 생각
	//num은 1 byte 값이며, 해당 값을 byte 타입으로 huffman_table.hbs 파일에 write
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
	code.open("huffman_code.hbs", ios::binary); //huffman_code.hbs를 binary 파일 형태로 오픈

	string huffman_code = "";
	for (unsigned int i = 0; i < store.size(); i++)
	{
		//store vector의 각 자리 문자와 vvv vector의 char를 비교하여 일치하면, 그 때의 huffman code 값을 huffman_code string에 더해줌
		for (auto& it : vvv)
		{
			if (store[i] == it.first.first)
			{
				huffman_code += it.second;
				continue;
			}
		}
	}
	//byte align을 위해 모자라는 bit만큼 채워줌
	int stuffingbit = 8 - (huffman_code.length() % 8);
	for (int i = 0; i < stuffingbit; i++)
		huffman_code += "0";

	//huffman_code string의 한 문자를 한 bit로 생각
	//num은 1 byte 값이며, 해당 값을 byte 타입으로 huffman_code.hbs 파일에 write
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
	cout << "압축률(Compression rate) = " << compression << "%" << endl;
}

inline double filesize(const char* filename)
{
	ifstream fp;
	fp.open(filename);
	fp.seekg(0, ios::end);
	double filesize = (double)fp.tellg(); //filesize 구하는 함수
	return filesize;
}