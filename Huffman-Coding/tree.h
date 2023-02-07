#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <string>
using namespace std;

class node
{
private:
	char c;
	int prob;
	node* left;
	node* right;
	node* parent;
public:
	node() { left = NULL; right = NULL; parent = NULL; c = 0; prob = 0; } //기본 생성자
	node(char cin, int num) //복사 생성자
	{
		left = NULL; right = NULL; parent = NULL; c = cin; prob = num;
	}
	node(node* Left, node* Right, node* Parent) //복사 생성자
	{
		left = Left; right = Right; parent = Parent;
		prob = Left->prob + Right->prob; c = 0;
	}
	~node() { delete(left); delete(right); } //소멸자
	char getchar() { return c; }
	int getprob() { return prob; }
	node* getLeft() { return left; }
	node* getRight() { return right; }
	node* getParent() { return parent; }
	void setLeft(node* nLeft) { left = nLeft; }
	void setRight(node* nRight) { right = nRight; }
	void setParent(node* nParent) { parent = nParent; }
};

class tree : public node
{
private:
	node* root;
public:
	tree() { root = NULL; } //생성자
	~tree() { } //소멸자
	node* getRoot() { return root; }
	void setRoot(node* Root) { root = Root; }

	//huffman tree 생성
	vector <pair< pair<char, int>, string >> huffman(vector<pair<char, int>> vv, vector <pair< pair<char, int>, string >> vvv);
	//각 문자별 huffman code로 변환
	vector <pair< pair<char, int>, string >> getCode(vector <pair< pair<char, int>, string >> vvv);
};
