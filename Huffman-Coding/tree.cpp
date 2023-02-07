#include "tree.h"

inline vector <pair< pair<char, int>, string >> tree::huffman(vector<pair<char, int>> vv, vector <pair< pair<char, int>, string >> vvv)
{
	//내림차순으로 정렬된 huffman table(vv)의 값을 순서대로 넣어주어 작은 값들부터 차례로 다룰 수 있도록 함.
	stack<node*> s;
	for (auto& it : vv)
		s.push(new node(it.first, it.second));

	while (s.size() > 1)
	{
		node* left = s.top();
		s.pop();
		node* right = s.top();
		s.pop();

		node* newNode = new node(left, right, NULL); //left, right의 부모노드, prob = left.prob + right.prob
		left->setParent(newNode);
		right->setParent(newNode);

		stack<node*> s2; //newNode의 prob보다 값이 작은 노드들을 잠시 담아두기 위한 stack
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

	setRoot(s.top()); //stack에 마지막으로 남은 노드는 root
	s.pop();

	vvv = getCode(vvv); //생성한 huffman tree를 이용해 각 문자별 huffman code를 구함.

	return vvv;
}

inline vector <pair< pair<char, int>, string >> tree::getCode(vector <pair< pair<char, int>, string >> vvv)
{
	string str = "";
	stack < pair<node*, string>> st;
	st.push({ root, "" }); //root 자리의 문자열은 ""

	while (!st.empty())
	{
		node* cur = st.top().first;
		string tmp = st.top().second;
		st.pop();

		//현재 위치에서 오른쪽으로 가게될 경우, 문자열 맨 뒤에 '1' 추가
		if (cur->getRight())
			st.push({ cur->getRight(), tmp + "1" });
		//현재 위치에서 왼쪽으로 가게 될 경우, 문자열 맨 뒤에 '0' 추가
		if (cur->getLeft())
			st.push({ cur->getLeft(), tmp + "0" });
		//left node일 경우, 그때의 문자열이 vvv vector의 second로 저장된다.
		if ((!cur->getLeft()) && (!cur->getRight()))
			vvv.push_back(make_pair(make_pair(cur->getchar(), cur->getprob()), tmp));
	}

	return vvv;
}