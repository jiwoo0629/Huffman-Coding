#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <bitset>

using namespace std;

vector<pair<char, int>> makeV(vector<char>); //store�� �̿��� ���ں� ���� �����ϴ� vector ����
vector <pair<char, vector<pair<char, int>>>> makeVCA(vector<char>, vector<pair<char, int>>);
const char* huffman_table(vector <pair< pair<char, int>, string >>);
const char* huffman_code(vector<char>, vector <pair< pair<char, int>, string >>);
const char* CAhuffman_table(vector <pair<char, vector<pair<pair<char, int>, string>>>>); //dontext_adaptive_huffman_table.hbs�� ��ȯ
const char* CAhuffman_code(const char*, vector<char>, vector <pair< pair<char, int>, string >>, vector<pair<char, vector<pair<pair<char, int>, string>>>>); //huffman_code.hbs�� ��ȯ
void compression(const char*, const char*, const char*, const char*, const char*); //������ ����� ���

bool compVV(pair<char, int>, pair<char, int>); //vv vector �������� ����
bool compVVV(pair<pair<char, int>, string>, pair<pair<char, int>, string>); //vvv vector �������� ����
bool compVVCA(pair<char, vector<pair<char, int>>>, pair<char, vector<pair<char, int>>>); //vvCA vector �������� ����
double filesize(const char*); //filesize ���