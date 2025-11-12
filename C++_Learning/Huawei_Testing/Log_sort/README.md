# 题目描述

采集到某产品线网运行一天产生的日志n条，现需根据日志时间先后顺序对日志进行排序，日志时间格式为H:M:S.N。

H表示小时(0~23)
M表示分钟(0~59)
S表示秒(0~59)
N表示毫秒(0~999)
时间可能并没有补全，也就是说，01:01:01.001也可能表示为1:1:1.1。

输入描述
第一行输入一个整数n表示日志条数，1<=n<=100000，接下来n行输入n个时间。

输出描述
按时间升序排序之后的时间，如果有两个时间表示的时间相同，则保持输入顺序。

示例1
输入
```
2
01:41:8.9
1:1:09.211
```
输出
```
1:1:09.211
01:41:8.9
```
说明

示例2
输入
```
3
23:41:08.023
1:1:09.211
08:01:22.0
```
输出
```
1:1:09.211
08:01:22.0
23:41:08.023
```
说明

示例3
输入
```
2
22:41:08.023
22:41:08.23
```
输出
```
22:41:08.023
22:41:08.23
```
说明 两个时间表示的时间相同，保持输入顺序

# 重点代码部分

## 把"22：41：08.012"格式变化为数字
```
#include<regex>

int convertTime(string timestr){
    regex pattern("(\\d+):(\\d+):(\\d+).(\\d+)");
    smatch matcher;
    if(regex.serch(timestr,matcher,pattern)!=false){
        return ((stoi(matcher[1]) * 60 + stoi(matcher[2])) * 60 + stoi(matcher[3])) * 1000 + stoi(matcher[4]);
    }
    return 0;
}

```

## 排序规则
```
sort(Times.begin(), Times.end(), [](const Time& Time1, const Time& Time2) {
		return Time1.t < Time2.t;
		});
```

## 完整代码
```
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<regex>
#include<algorithm>
using namespace std;

class Time {
public:
	int t;
	string timestr;
	Time() = default;
	Time(int t,string timestr)
		:t(t),timestr(timestr){}
};

int convertTime(string timestr) {
	//h:m:s.n
	regex pattren("(\\d+):(\\d+):(\\d+).(\\d+)");
	smatch matcher;
	if (regex_search(timestr, matcher, pattren) != false) {
		return ((stoi(matcher[1]) * 60 + stoi(matcher[2])) * 60 + stoi(matcher[3])) * 1000 + stoi(matcher[4]);
	}
	return 0;
}

int main() {
	int n;
	cin >> n;
	vector<Time> Times;
	for (int i = 0; i < n; i++) {
		string timestr;
		cin >> timestr;
		Times.emplace_back(convertTime(timestr), timestr);
	}
	sort(Times.begin(), Times.end(), [](const Time& Time1, const Time& Time2) {
		return Time1.t < Time2.t;
		});
	for (auto t : Times) {
		cout << t.timestr << endl;
	}
	return 0;
}
```