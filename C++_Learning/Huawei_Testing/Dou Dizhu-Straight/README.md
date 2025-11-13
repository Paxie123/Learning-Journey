# 斗地主 - 顺子

## 题目描述
```
在斗地主扑克牌游戏中， 扑克牌由小到大的顺序为：3,4,5,6,7,8,9,10,J,Q,K,A,2，玩家可以出的扑克牌阵型有：单张、对子、顺子、飞机、炸弹等。

其中顺子的出牌规则为：由至少5张由小到大连续递增的扑克牌组成，且不能包含2。

例如：{3,4,5,6,7}、{3,4,5,6,7,8,9,10,J,Q,K,A}都是有效的顺子；而{J,Q,K,A,2}、 {2,3,4,5,6}、{3,4,5,6}、{3,4,5,6,8}等都不是顺子。

给定一个包含13张牌的数组，如果有满足出牌规则的顺子，请输出顺子。

如果存在多个顺子，请每行输出一个顺子，且需要按顺子的第一张牌的大小（必须从小到大）依次输出。

如果没有满足出牌规则的顺子，请输出No。
```
## 输入描述
```
13张任意顺序的扑克牌，每张扑克牌数字用空格隔开，每张扑克牌的数字都是合法的，并且不包括大小王：

2 9 J 2 3 4 K A 7 9 A 5 6

不需要考虑输入为异常字符的情况
```
## 输出描述
```
组成的顺子，每张扑克牌数字用空格隔开：

3 4 5 6 7
```
## 示例
输入
```
2 9 J 2 3 4 K A 7 9 A 5 6
```
输出
```
3 4 5 6 7
# 13张牌中，可以组成的顺子只有1组：3 4 5 6 7。
```


示例2
输入：
```
2 9 J 10 3 4 K A 7 Q A 5 6
```
输出：
```
3 4 5 6 7
9 10 J Q K A
```
说明: 13张牌中，可以组成2组顺子，从小到大分别为：3 4 5 6 7 和 9 10 J Q K A

示例3
输入：
```
2 9 9 9 3 4 K A 10 Q A 5 6
```
输出：
```
No
```
说明: 13张牌中，无法组成顺子。

# 解题思路

## 输入部分
```
# 本题输入为一行13个的数字或字母，因此输入不能简单的为整数输入，接收输入时使用字符串，采用常规的单行输入

string input;
getline(cin, input);
```
## 输入数据处理
```
# 因为要对比数字，所以决定把J Q K A 2 作为数字 11 12 13 14 16。

# 创造类，分别存储字符串和数字
class Card {
public:
	string s;
	int i;
	Card(string s, int i)
		:s(s), i(i) {}
};

# 把输入初始化进此类的数组里面
map<string, int> tmp = {
	{"3", 3}, {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7},
	{"8", 8}, {"9", 9}, {"10", 10}, {"J", 11}, {"Q", 12},
	{"K", 13}, {"A", 14}, {"2", 16}
};
istringstream iss(input);
vector<Card> cards;
string c;
while (iss >> c) {
	cards.emplace_back(c, tmp[c]);
}

# 排序
sort(cards.begin(), cards.end(), [](Card a, Card b) {return a.i < b.i; });
```
## 形成顺子
```
# vector<vector<Card>> qs 定义一个顺子的数组
# 后面让cards里面每一张牌都去看能不能放在某个数组的后面，如果可以，就放在数组后面，如果不行，则重新再开一个数组放在qs最后
vector<vector<Card>> qs;
vector<Card> c_q;
c_q.push_back(cards[0]);
qs.push_back(c_q);
for (int i = 1; i < cards.size(); i++) {
	for (auto& q : qs) {
		if (cards[i].i - q[q.size()-1].i == 1) {
			q.push_back(cards[i]);
			break;
		}
	}
	vector<Card> new_q;
	new_q.push_back(cards[i]);
	qs.push_back(new_q);
}

# 完成后检查qs中大于5个的数组
vector<vector<Card>> valid_q;
for (auto& q : qs) {
	if (q.size() >= 5) {
		valid_q.push_back(q);
	}
}
if (valid_q.empty()) {
	cout << "No" << endl;
}
else {
	for (auto& q : valid_q) {
		for (auto& c : q) {
			cout << c.s << " ";
		}
		cout << endl;
	}
}
```

## 完整代码
```
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>

using namespace std;
class Card {
public:
	string s;
	int i;
	Card(string s, int i)
		:s(s), i(i) {}
};

int main() {
	map<string, int> tmp = {
		{"3", 3}, {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7},
		{"8", 8}, {"9", 9}, {"10", 10}, {"J", 11}, {"Q", 12},
		{"K", 13}, {"A", 14}, {"2", 16}
	};
	string input;
	getline(cin, input);
	istringstream iss(input);
	vector<Card> cards;
	string c;
	while (iss >> c) {
		cards.emplace_back(c, tmp[c]);
	}

	sort(cards.begin(), cards.end(), [](Card a, Card b) {return a.i < b.i; });
	vector<vector<Card>> qs;
	vector<Card> c_q;
	c_q.push_back(cards[0]);
	qs.push_back(c_q);
	for (int i = 1; i < cards.size(); i++) {
		for (auto& q : qs) {
			if (cards[i].i - q[q.size()-1].i == 1) {
				q.push_back(cards[i]);
				break;
			}
		}
		vector<Card> new_q;
		new_q.push_back(cards[i]);
		qs.push_back(new_q);
	}

	vector<vector<Card>> valid_q;
	for (auto& q : qs) {
		if (q.size() >= 5) {
			valid_q.push_back(q);
		}
	}
	if (valid_q.empty()) {
		cout << "No" << endl;
	}
	else {
		for (auto& q : valid_q) {
			for (auto& c : q) {
				cout << c.s << " ";
			}
			cout << endl;
		}
	}

	return 0;
}

```