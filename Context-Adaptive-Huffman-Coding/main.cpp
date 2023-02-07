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
	//filename �����ϴ� �κ�
	const char* inputname = "training_input.txt";
	const char* outputname = "training_output.txt";
	const char* CAcodename = "training_input_code.hbs";

	ifstream finput(inputname); //input_data.txt ���� ����

	vector<char> store; //input_data.txt ������ ���� char�� vector
	char c;
	while (finput.get(c))
		store.push_back(c);

	vector<pair<char, int>> vv; //���ں� ������ ������ vector(huffman table)
	vv = makeV(store); //huffman table
	sort(vv.begin(), vv.end(), compVV); //������������ ����

	vector <pair< pair<char, int>, string >> vvvE; //���ں� huffman code�� encoding�� ����� ������ vector
	tree* t = new tree();
	vvvE = t->huffman(vv, vvvE); //huffman encoding
	sort(vvvE.begin(), vvvE.end(), compVVV); //������������ ����

	vector <pair<char, vector<pair<char, int>>>> vvCA; //���ƿ� ���� ���ں� ������ ������ vector(C.A huffman table)
	vvCA = makeVCA(store, vv); //C.A huffman table
	for (auto& it : vvCA) //������������ ����
		sort(it.second.begin(), it.second.end(), compVV);
	
	vector <pair<char, vector<pair<pair<char,int>, string>> >> vvCAE; 
	for (int i = 0; i < vvCA.size(); i++)
	{
		tree* newt = new tree();
		vector <pair< pair<char, int>, string >> vvv;
		vvv = newt->huffman(vvCA[i].second, vvv); //�� ���� ���� ���ƿ� ���� huffman encoding
		sort(vvv.begin(), vvv.end(), compVVV); //������������ ����
		vvCAE.push_back(make_pair(vvCA.at(i).first, vvv)); //huffman codeword�� ������ huffman table 
	}

	for (int i = 0; i < vvCAE.size(); i++)
	{
		cout << vvCAE[i].first << "�� ���� context adaptive huffman coding" << endl;
		for (int j = 0; j < vvCAE[i].second.size(); j++)
			cout << vvCAE[i].second[j].first.first << "\t\t" << vvCAE[i].second[j].first.second << "\t\t" << vvCAE[i].second[j].second << endl;
		cout << endl;
	}

	const char* table = huffman_table(vvvE);
	const char* code = huffman_code(store, vvvE);
	const char* CAtable = CAhuffman_table(vvCAE); //dontext_adaptive_huffman_table.hbs�� ��ȯ
	const char* CAcode = CAhuffman_code(CAcodename, store, vvvE, vvCAE); //test_input_code.hbs�� ��ȯ
	cout << "Encoding �Ϸ�" << endl << endl;

	compression(table, code, CAtable, CAcode, inputname);
	
	//reduced CAtable ������ �׿� ���� CAcode ���� ����
	
	string huffman_table = readfile(table);
	vector<pair<pair<char, int>, string>> vvvD;
	vvvD = table_decoding(huffman_table, vvvD); //huffman_table string�� ������ �̿��� huffman table ����

	string CAhuffman_table = readfile(CAtable);
	vector <pair<char, vector<pair<pair<char, int>, string>> >> vvvCAD;
	vvvCAD = CAtable_decoding(CAhuffman_table, vvvCAD);

	string CAhuffman_code = readfile(CAcode);

	string CAoutput = code_decoding(CAhuffman_code, vvvD, vvvCAD);

	//training_output.txt ���Ͽ� ���
	ofstream foutput(outputname);
	foutput << CAoutput;

	cout << endl;
	cout << "Decoding �Ϸ�" << endl;

	foutput.close();
	finput.close();
	return 0;
}