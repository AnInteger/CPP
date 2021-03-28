#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

void getIp(vector<int>& v, string s) {
	int n = s.size();
	string tmp;

	for (int i = 0; i < n; i++) {
		tmp += s[i];
		if (s[i] == '.' || s[i] == '/' || i == n - 1) {
			v.push_back(atoi(tmp.c_str()));
			tmp.clear();
		}
	}
}

void tostring(vector<int> v, string &s) {
	string tmp;
	for (int j = 0; j < 4; j++) {

		if (v[j] < 0 || v[j] >255) {
			assert(false);
		}

		for (int i = 0; i < 8; i++) {
			if (v[j] % 2 == 0) {
				tmp += '0';
			}
			else {
				tmp += '1';
			}

			v[j] = v[j] / 2;
		}

		reverse(tmp.begin(), tmp.end());
		s += tmp;
		tmp.clear();
	}
	//cout << s << endl;
}

bool isSame(string p1, string p2, int k1, int k2) {
	if (k1 != k2) return false;
	return p1.substr(1, k1) == p2.substr(1, k2);
}

int main() {
	string s1 ="192.168.0.1/22", s2 = "192.168.0.2/22";
	vector<int> v1, v2;

	getIp(v1, s1);
	getIp(v2, s2);

	string compare1,compare2;

	tostring(v1, compare1);

	tostring(v2, compare2);

	if (isSame(compare1.c_str(), compare2.c_str(), v1[4], v2[4])) {
		cout << "is Same" << endl;
	}
	return 0;
}