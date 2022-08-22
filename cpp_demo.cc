#include <bits/stdc++.h>

using namespace std;


string solution(string &s) {
	unordered_map<char, int> mp;
	for(auto c : s) mp[c] ++;
	string ans;
	for(char c = '9'; c >= '0'; -- c) {
		if(mp[c] > 1) {
			if(c == '0' && ans.size() == 0) continue;
			int cnt = mp[c] / 2;
			ans += string(cnt, c);
			mp[c] -= cnt * 2;
		}
	}
	if(ans.size() == 0) {
		for(char c = '9'; c >= '0'; -- c) {
			if(mp[c] > 0) {
				return string(1, c);
			}
		}
	}else {
		char mid = 0;
		for(char c = '9'; c >= '0'; -- c) {
			if(mp[c] == 1) {
				mid = c;
				break;
			}
		}
		string tmp = ans;
		reverse(tmp.begin(), tmp.end());
		if(mid != 0) {
			return ans + string(1, mid) + tmp;
		}
		return ans + tmp;
	}
	return "";
}

int main(){
	string s;
	cin >> s;
	cout << solution(s) << endl;
	return 0;
}