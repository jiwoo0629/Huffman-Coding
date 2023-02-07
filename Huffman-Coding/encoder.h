#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <bitset>

using namespace std;

vector<pair<char, int>> makeV(vector<char> store); //store�� �̿��� ���ں� ���� �����ϴ� vector ����
const char* huffman_table(vector <pair< pair<char, int>, string >>); //huffman_table.hbs�� ��ȯ
const char* huffman_code(vector<char>, vector <pair< pair<char, int>, string >>); //huffman_code.hbs�� ��ȯ
void compression(const char*, const char*, const char*); //������ ����� ���

bool compVV(pair<char, int>, pair<char, int>); //vv vector �������� ����
bool compVVV(pair<pair<char, int>, string>, pair<pair<char, int>, string>); //vvv vector �������� ����
double filesize(const char*); //filesize ���

