#pragma once
#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

string readfile(const char* filename); //binary ������ ������ �о�� string ���·� ����
vector<pair<pair<char, int>, string>> table_decoding(string, vector<pair<pair<char, int>, string>>); //���� string�� �̿��� huffman_table ����
vector<int> getlen(vector<pair<pair<char, int>, string>>); //bit ���� ������ �����ϴ� vector
string code_decoding(string, vector<pair<pair<char, int>, string>>, int); //���� string�� decoding �Ͽ� ���� ���� text�� string ���·� ����
bool compare(pair<pair<char, int>, string>, pair<pair<char, int>, string>); //bit ���� �������� �������� ����
