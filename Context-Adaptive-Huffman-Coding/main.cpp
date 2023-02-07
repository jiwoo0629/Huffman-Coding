#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <bitset>

#include "tree.cpp"
#include "encoder.cpp"
#include "decoder.cpp"

using namespace std;

int main(void)
{
	//filename 수정하는 부분
	const char* inputname = "training_input.txt";
	const char* outputname = "training_output.txt";
	const char* CAcodename = "training_input_code.hbs";

	ifstream finput(inputname); //input_data.txt 파일 오픈

	vector<char> store; //input_data.txt 내용을 담을 char형 vector
	char c;
	while (finput.get(c))
		store.push_back(c);

	vector<pair<char, int>> vv; //문자별 개수를 저장할 vector(huffman table)
	vv = makeV(store); //huffman table
	sort(vv.begin(), vv.end(), compVV); //내림차순으로 정렬

	vector <pair< pair<char, int>, string >> vvvE; //문자별 huffman code로 encoding한 결과를 저장할 vector
	tree* t = new tree();
	vvvE = t->huffman(vv, vvvE); //huffman encoding
	sort(vvvE.begin(), vvvE.end(), compVVV); //오름차순으로 정렬

	vector <pair<char, vector<pair<char, int>>>> vvCA; //문맥에 따른 문자별 개수를 저장할 vector(C.A huffman table)
	vvCA = makeVCA(store, vv); //C.A huffman table
	for (auto& it : vvCA) //내림차순으로 정렬
		sort(it.second.begin(), it.second.end(), compVV);
	
	vector <pair<char, vector<pair<pair<char,int>, string>> >> vvCAE; 
	for (int i = 0; i < vvCA.size(); i++)
	{
		tree* newt = new tree();
		vector <pair< pair<char, int>, string >> vvv;
		vvv = newt->huffman(vvCA[i].second, vvv); //각 문자 별로 문맥에 따른 huffman encoding
		sort(vvv.begin(), vvv.end(), compVVV); //오름차순으로 정렬
		vvCAE.push_back(make_pair(vvCA.at(i).first, vvv)); //huffman codeword를 포함한 huffman table 
	}

	for (int i = 0; i < vvCAE.size(); i++)
	{
		cout << vvCAE[i].first << "에 대한 context adaptive huffman coding" << endl;
		for (int j = 0; j < vvCAE[i].second.size(); j++)
			cout << vvCAE[i].second[j].first.first << "\t\t" << vvCAE[i].second[j].first.second << "\t\t" << vvCAE[i].second[j].second << endl;
		cout << endl;
	}

	const char* table = huffman_table(vvvE);
	const char* code = huffman_code(store, vvvE);
	const char* CAtable = CAhuffman_table(vvCAE); //dontext_adaptive_huffman_table.hbs로 변환
	const char* CAcode = CAhuffman_code(CAcodename, store, vvvE, vvCAE); //test_input_code.hbs로 변환
	cout << "Encoding 완료" << endl << endl;

	compression(table, code, CAtable, CAcode, inputname);
	
	//reduced CAtable 만들어보고 그에 따라 CAcode 새로 얻어보기
	
	string huffman_table = readfile(table);
	vector<pair<pair<char, int>, string>> vvvD;
	vvvD = table_decoding(huffman_table, vvvD); //huffman_table string의 내용을 이용해 huffman table 구현

	string CAhuffman_table = readfile(CAtable);
	vector <pair<char, vector<pair<pair<char, int>, string>> >> vvvCAD;
	vvvCAD = CAtable_decoding(CAhuffman_table, vvvCAD);

	string CAhuffman_code = readfile(CAcode);

	string CAoutput = code_decoding(CAhuffman_code, vvvD, vvvCAD);

	//training_output.txt 파일에 출력
	ofstream foutput(outputname);
	foutput << CAoutput;

	cout << endl;
	cout << "Decoding 완료" << endl;

	foutput.close();
	finput.close();
	return 0;
}