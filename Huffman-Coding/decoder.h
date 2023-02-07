#pragma once
#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

string readfile(const char* filename); //binary 파일의 내용을 읽어와 string 형태로 저장
vector<pair<pair<char, int>, string>> table_decoding(string, vector<pair<pair<char, int>, string>>); //얻은 string을 이용해 huffman_table 구축
vector<int> getlen(vector<pair<pair<char, int>, string>>); //bit 수의 종류를 저장하는 vector
string code_decoding(string, vector<pair<pair<char, int>, string>>, int); //얻은 string을 decoding 하여 얻은 원래 text를 string 형태로 저장
bool compare(pair<pair<char, int>, string>, pair<pair<char, int>, string>); //bit 수를 기준으로 오름차순 정렬
