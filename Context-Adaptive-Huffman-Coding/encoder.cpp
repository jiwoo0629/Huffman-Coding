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

inline vector <pair<char, vector<pair<char, int>>>> makeVCA(vector<char> store, vector<pair<char, int>> vv)
{
	vector <pair<char, vector<pair<char, int>>>> vvCA; //C.A huffman table의 기본 틀 생성
	for (int i = 0; i < vv.size(); i++)
	{
		vector <pair<char, int>> v;
		for (int j = 0; j < vv.size(); j++)
			v.push_back(make_pair(vv.at(j).first, 0));
		vvCA.push_back(make_pair(vv.at(i).first, v));
	}
	char cur = 0, prev = 0; //기존 symbol과 현재 symbol을 담을 변수
	for (unsigned int i = 0; i < store.size(); i++)
	{
		if (i == 0)
		{	
			cur = prev = store[0];
			continue;
		}
		cur = store[i]; //현재 
		for (char j = 0; j < 127; j++)
		{
			if (prev == j)
			{
				//문자가 일치할 경우 해당 문자의 개수 추가
				for (auto& it : vvCA)
				{
					if (it.first == prev)
					{
						vector<pair<char, int>> vv2 = it.second;
						for (auto& it2 : vv2)
						{
							if (it2.first == cur)
							{
								it2.second++;
								it.second = vv2;
								prev = it2.first;
								break;
							}
						}
						break;
					}
				}
			}
		}
	}
	return vvCA;
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
	int stuffingbit = 8 - (huffman_table.size() % 8);
	if (stuffingbit != 8)
	{
		for (int i = 0; i < stuffingbit; i++)
			huffman_table += '0';
	}
	
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
	int stuffingbit = 8 - (huffman_code.size() % 8);
	if (stuffingbit != 8)
	{
		for (int i = 0; i < stuffingbit; i++)
			huffman_code += '0';
	}

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

inline const char* CAhuffman_table(vector <pair<char, vector<pair<pair<char, int>, string>>>> vvCAE)
{
	ofstream table;
	table.open("context_adaptive_huffman_table.hbs", ios::binary); //huffman_table.hbs를 binary 파일 형태로 오픈
	string CAhuffman_table = "";
	for (unsigned int i = 0; i < vvCAE.size(); i++)
	{
		//vvCAE vector의 char, int를 각각 8 bitset으로 생각해 문자열로 변환해 huffman_table string에 더함
		for (int j = 0; j < vvCAE[i].second.size(); j++)
		{
			CAhuffman_table += bitset<8>(vvCAE[i].first).to_string();
			CAhuffman_table += bitset<8>(vvCAE[i].second[j].first.first).to_string();
			int length = (vvCAE[i].second[j].second).size();
			CAhuffman_table += bitset<8>(length).to_string();
			CAhuffman_table += vvCAE[i].second[j].second;
		}
	}
	//byte align을 위해 모자라는 bit만큼 채워줌
	int stuffingbit = 8 - (CAhuffman_table.size() % 8);
	if (stuffingbit != 8)
	{
		for (int i = 0; i < stuffingbit; i++)
			CAhuffman_table += '0';
	}

	//CAhuffman_table string의 한 문자를 한 bit로 생각
	//num은 1 byte 값이며, 해당 값을 byte 타입으로 huffman_table.hbs 파일에 write
	for (unsigned int i = 0; i < (CAhuffman_table.size() / 8); i++)
	{
		int n = 0;
		for (int j = 0; j < 8; j++)
		{
			int at = 8 * i + j;
			char c = CAhuffman_table.at(at);
			int num = c - '0';
			n += (num * (int)pow(2, (7 - j)));
		}
		table.write((char*)&n, 1);
	}
	table.close();
	const char* CAtable = "context_adaptive_huffman_table.hbs";
	return CAtable;
}

inline const char* CAhuffman_code(const char* CAcodename, vector<char> store, vector <pair< pair<char, int>, string >> vvv, vector <pair<char, vector<pair<pair<char, int>, string>>>> vvCAE)
{
	ofstream code;
	code.open(CAcodename, ios::binary); //huffman_code.hbs를 binary 파일 형태로 오픈

	string CAhuffman_code = "";
	char prev = 0;
	//store vector의 각 자리 문자와 vvv vector의 char를 비교하여 일치하면, 그 때의 huffman code 값을 huffman_code string에 더해줌
	for (auto& it : vvv)
	{
		if (store[0] == it.first.first)
		{
			CAhuffman_code += it.second;
			prev = it.first.first;
			continue;
		}
	}
	for (unsigned int i = 1; i < store.size(); i++)
	{
		int find = 0;
		//store vector의 각 자리 문자와 vvCAE vector의 char를 비교하여 일치하면, 그 때의 huffman code 값을 huffman_code string에 더해줌
		for (auto& it : vvCAE)
		{
			if (prev == it.first) //기존 문자를 찾는다
			{
				vector<pair<pair<char, int>, string>> vCAE = it.second;
				for (auto& it2 : vCAE) //현재 문자를 찾는다
				{
					if (store[i] == it2.first.first)
					{
						CAhuffman_code += it2.second;
						prev = it2.first.first;
						find = 1;
						break;
					}
				}
			}
			if (find == 1)
				break;
		}
	}
	//byte align을 위해 모자라는 bit만큼 채워줌
	int stuffingbit = 8 - (CAhuffman_code.size() % 8);
	if (stuffingbit != 8) 
	{
		for (int i = 0; i < stuffingbit; i++)
			CAhuffman_code += '0';
	}
	
	//huffman_code string의 한 문자를 한 bit로 생각
	//num은 1 byte 값이며, 해당 값을 byte 타입으로 huffman_code.hbs 파일에 write
	for (unsigned int i = 0; i < (CAhuffman_code.size() / 8); i++)
	{
		int n = 0;
		for (int j = 0; j < 8; j++)
		{
			int at = 8 * i + j;
			char c = CAhuffman_code.at(at);
			int num = c - '0';
			n += (num * (int)pow(2, (7 - j)));
		}
		code.write((char*)&n, 1);
	}
	code.close();
	return CAcodename;
}

inline bool compVV(pair<char, int> a, pair<char, int> b)
{
	return a.second > b.second;
}

inline bool compVVV(pair<pair<char, int>, string> a, pair<pair<char, int>, string> b)
{
	return a.first.second > b.first.second;
}

//메일 답장 오면 compression, cost 식 업데이트하기
inline void compression(const char* table, const char* code, const char* CAtable, const char* CAcode, const char* input)
{
	double tablefilesize = filesize(table); //huffman_table.hbs file size
	double codefilesize = filesize(code);
	double CAtablefilesize = filesize(CAtable);
	double CAcodefilesize = filesize(CAcode); //huffman_code.hbs file size
	double inputfilesize = filesize(input); //input_data.txt file size

	double compression = (1 - ((tablefilesize + CAtablefilesize + CAcodefilesize) / inputfilesize)) * 100; //압축률 : (1 - (encoded byte / input byte)) * 100% 
	double cost = CAcodefilesize + 0.001 * (tablefilesize + CAtablefilesize); //Cost : encoded byte + 0.001 * table byte 값이 최소가 되려면 encoded byte가 최소가 되어야 함.

	cout << "Input size : " << inputfilesize << endl;
	cout << "Huffman table size : " << tablefilesize << endl;
	cout << "Huffman code size : " << codefilesize << endl;
	cout << "Context Adaptive huffman table size : " << CAtablefilesize << endl;
	cout << "Context Adaptive huffman code size : " << CAcodefilesize << endl;
	cout << "Compression rate = " << compression << "%" << endl;
	cout << "Cost = " << cost << endl;
}

inline double filesize(const char* filename)
{
	ifstream fp;
	fp.open(filename);
	fp.seekg(0, ios::end);
	double filesize = (double)fp.tellg(); //filesize 구하는 함수
	return filesize;
}