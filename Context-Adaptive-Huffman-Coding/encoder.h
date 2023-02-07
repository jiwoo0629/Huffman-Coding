#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <bitset>

using namespace std;

vector<pair<char, int>> makeV(vector<char>); //store를 이용해 문자별 개수 구분하는 vector 생성
vector <pair<char, vector<pair<char, int>>>> makeVCA(vector<char>, vector<pair<char, int>>);
const char* huffman_table(vector <pair< pair<char, int>, string >>);
const char* huffman_code(vector<char>, vector <pair< pair<char, int>, string >>);
const char* CAhuffman_table(vector <pair<char, vector<pair<pair<char, int>, string>>>>); //dontext_adaptive_huffman_table.hbs로 변환
const char* CAhuffman_code(const char*, vector<char>, vector <pair< pair<char, int>, string >>, vector<pair<char, vector<pair<pair<char, int>, string>>>>); //huffman_code.hbs로 변환
void compression(const char*, const char*, const char*, const char*, const char*); //파일의 압축률 계산

bool compVV(pair<char, int>, pair<char, int>); //vv vector 내림차순 정렬
bool compVVV(pair<pair<char, int>, string>, pair<pair<char, int>, string>); //vvv vector 오름차순 정렬
bool compVVCA(pair<char, vector<pair<char, int>>>, pair<char, vector<pair<char, int>>>); //vvCA vector 내림차순 정렬
double filesize(const char*); //filesize 계산