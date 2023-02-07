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
	node() { left = NULL; right = NULL; parent = NULL; c = 0; prob = 0; } //�⺻ ������
	node(char cin, int num) //���� ������
	{
		left = NULL; right = NULL; parent = NULL; c = cin; prob = num;
	}
	node(node* Left, node* Right, node* Parent) //���� ������
	{
		left = Left; right = Right; parent = Parent;
		prob = Left->prob + Right->prob; c = 0;
	}
	~node() { delete(left); delete(right); } //�Ҹ���
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
	tree() { root = NULL; } //������
	~tree() { } //�Ҹ���
	node* getRoot() { return root; }
	void setRoot(node* Root) { root = Root; }

	//huffman tree ����
	vector <pair< pair<char, int>, string >> huffman(vector<pair<char, int>> vv, vector <pair< pair<char, int>, string >> vvv);
	//�� ���ں� huffman code�� ��ȯ
	vector <pair< pair<char, int>, string >> getCode(vector <pair< pair<char, int>, string >> vvv);
};
