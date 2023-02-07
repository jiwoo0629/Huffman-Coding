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
	tree* t = new tree(); //Huffman tree를 위해 tree 생성

	ifstream finput;
	finput.open("input_data.txt"); //input_data.txt 파일 오픈

	vector<char> store; //input_data.txt 내용을 담을 char형 vector
	char c;
	while (finput.get(c)) //한 글자마다 저장
		store.push_back(c);

	vector<pair<char, int>> vv; //문자별 개수를 저장할 vector(huffman table)
	vv = makeV(store);
	sort(vv.begin(), vv.end(), compVV); //내림차순으로 정렬

	vector <pair< pair<char, int>, string >> vvvE; //문자별 huffman code로 encoding한 결과를 저장할 vector
	vvvE = t->huffman(vv, vvvE); //huffman encoding
	sort(vvvE.begin(), vvvE.end(), compVVV); //오름차순으로 정렬

	cout << "Huffman Tree" << endl;
	cout << "char\t\tcount\t\thuffman code" << endl;
	for (unsigned int i = 0; i < vvvE.size(); i++)
		cout << "' " << vvvE[i].first.first << " '\t\t" << vvvE[i].first.second << "\t\t" << vvvE[i].second << endl;

	const char* tablename = huffman_table(vvvE); //huffman_table.hbs로 변환
	const char* codename = huffman_code(store, vvvE); //huffman_code.hbs로 변환
	
	cout << endl;
	cout << "Encoding 완료" << endl << endl;
	compression(tablename, codename, "input_data.txt"); //파일의 압축률 표시

	string huffman_table = readfile(tablename); //huffman_table.hbs의 내용을 읽어와 string 형태로 저장
	
	vector<pair<pair<char, int>, string>> vvvD; 
	vvvD = table_decoding(huffman_table, vvvD); //huffman_table string의 내용을 이용해 huffman table 구현
	vector<int> len = getlen(vvvD); //bit의 값의 종류를 저장하는 vector

	string huffman_code = readfile(codename); //huffman_code.hbs의 내용을 읽어와 string 형태로 저장

	int length = len.back(); //length가 가장 큰 값
	string output = code_decoding(huffman_code, vvvD, length); //huffman_code string의 내용을 decoding한 결과를 string 형태로 저장

	//output.txt 파일에 출력
	ofstream foutput;
	foutput.open("output.txt"); 
	foutput << output;
	
	cout << endl;
	cout << "Decoding 완료" << endl;

	foutput.close();
	finput.close();
	return 0;
}