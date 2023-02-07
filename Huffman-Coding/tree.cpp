#include "tree.h"

inline vector <pair< pair<char, int>, string >> tree::huffman(vector<pair<char, int>> vv, vector <pair< pair<char, int>, string >> vvv)
{
	//������������ ���ĵ� huffman table(vv)�� ���� ������� �־��־� ���� ������� ���ʷ� �ٷ� �� �ֵ��� ��.
	stack<node*> s;
	for (auto& it : vv)
		s.push(new node(it.first, it.second));

	while (s.size() > 1)
	{
		node* left = s.top();
		s.pop();
		node* right = s.top();
		s.pop();

		node* newNode = new node(left, right, NULL); //left, right�� �θ���, prob = left.prob + right.prob
		left->setParent(newNode);
		right->setParent(newNode);

		stack<node*> s2; //newNode�� prob���� ���� ���� ������ ��� ��Ƶα� ���� stack
		if (!s.empty())
		{
			while (s.top()->getprob() <= newNode->getprob())
			{
				s2.push(s.top());
				s.pop();
				if (s.empty())
					break;
			}
			s.push(newNode);
			while (!s2.empty())
			{
				s.push(s2.top());
				s2.pop();
			}
		}
		else
			s.push(newNode);
	}

	setRoot(s.top()); //stack�� ���������� ���� ���� root
	s.pop();

	vvv = getCode(vvv); //������ huffman tree�� �̿��� �� ���ں� huffman code�� ����.

	return vvv;
}

inline vector <pair< pair<char, int>, string >> tree::getCode(vector <pair< pair<char, int>, string >> vvv)
{
	string str = "";
	stack < pair<node*, string>> st;
	st.push({ root, "" }); //root �ڸ��� ���ڿ��� ""

	while (!st.empty())
	{
		node* cur = st.top().first;
		string tmp = st.top().second;
		st.pop();

		//���� ��ġ���� ���������� ���Ե� ���, ���ڿ� �� �ڿ� '1' �߰�
		if (cur->getRight())
			st.push({ cur->getRight(), tmp + "1" });
		//���� ��ġ���� �������� ���� �� ���, ���ڿ� �� �ڿ� '0' �߰�
		if (cur->getLeft())
			st.push({ cur->getLeft(), tmp + "0" });
		//left node�� ���, �׶��� ���ڿ��� vvv vector�� second�� ����ȴ�.
		if ((!cur->getLeft()) && (!cur->getRight()))
			vvv.push_back(make_pair(make_pair(cur->getchar(), cur->getprob()), tmp));
	}

	return vvv;
}