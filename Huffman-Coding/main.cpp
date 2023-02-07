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
	tree* t = new tree(); //Huffman tree�� ���� tree ����

	ifstream finput;
	finput.open("input_data.txt"); //input_data.txt ���� ����

	vector<char> store; //input_data.txt ������ ���� char�� vector
	char c;
	while (finput.get(c)) //�� ���ڸ��� ����
		store.push_back(c);

	vector<pair<char, int>> vv; //���ں� ������ ������ vector(huffman table)
	vv = makeV(store);
	sort(vv.begin(), vv.end(), compVV); //������������ ����

	vector <pair< pair<char, int>, string >> vvvE; //���ں� huffman code�� encoding�� ����� ������ vector
	vvvE = t->huffman(vv, vvvE); //huffman encoding
	sort(vvvE.begin(), vvvE.end(), compVVV); //������������ ����

	cout << "Huffman Tree" << endl;
	cout << "char\t\tcount\t\thuffman code" << endl;
	for (unsigned int i = 0; i < vvvE.size(); i++)
		cout << "' " << vvvE[i].first.first << " '\t\t" << vvvE[i].first.second << "\t\t" << vvvE[i].second << endl;

	const char* tablename = huffman_table(vvvE); //huffman_table.hbs�� ��ȯ
	const char* codename = huffman_code(store, vvvE); //huffman_code.hbs�� ��ȯ
	
	cout << endl;
	cout << "Encoding �Ϸ�" << endl << endl;
	compression(tablename, codename, "input_data.txt"); //������ ����� ǥ��

	string huffman_table = readfile(tablename); //huffman_table.hbs�� ������ �о�� string ���·� ����
	
	vector<pair<pair<char, int>, string>> vvvD; 
	vvvD = table_decoding(huffman_table, vvvD); //huffman_table string�� ������ �̿��� huffman table ����
	vector<int> len = getlen(vvvD); //bit�� ���� ������ �����ϴ� vector

	string huffman_code = readfile(codename); //huffman_code.hbs�� ������ �о�� string ���·� ����

	int length = len.back(); //length�� ���� ū ��
	string output = code_decoding(huffman_code, vvvD, length); //huffman_code string�� ������ decoding�� ����� string ���·� ����

	//output.txt ���Ͽ� ���
	ofstream foutput;
	foutput.open("output.txt"); 
	foutput << output;
	
	cout << endl;
	cout << "Decoding �Ϸ�" << endl;

	foutput.close();
	finput.close();
	return 0;
}