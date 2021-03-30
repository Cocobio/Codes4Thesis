#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main() {
	int n;

	cin >> n;

	set<int> tmp;
	
	for (int i=0; i<n; i++) {
		tmp.insert(i);
	}


	string str;
	getline(cin, str);

	int l = 2;


	while (getline(cin, str)) {
		vector<set<int>> functions;
		istringstream iss(str);
		string str2;

		for (int i=0; i<n; i++)
			functions.push_back(set<int>(tmp.begin(),tmp.end()));

		// cout << "iss" << str << endl;

		while (iss >> str2) {
			set<int> tmp_set;
			set<int> results;

			for (int i=0; i<n; i++) {
				if (str2[i] == '1')
					tmp_set.insert(i);
			}

			// cout << "tmp_set: ";
			// for(auto &algo : tmp_set)
			// 	cout << algo << " ";
			// cout << endl;

			for (auto &idx : tmp_set) {
				// cout << idx << endl;
				set_intersection(tmp_set.begin(), tmp_set.end(), functions[idx].begin(), functions[idx].end(), inserter(results, results.begin()));
				functions[idx].clear();
				functions[idx].insert(results.begin(), results.end());

				// cout << "f_" << idx;
				// for (auto &algo : functions[idx])
				// 	cout << " " << algo;
				// cout << endl;
				results.clear();
			}
		}

		bool flag = false;
		for (auto &f : functions) {
			// cout << f.size() << endl;
			if (f.size() > 1) {
				flag = true;
			}
		}

		if (flag) {
			cout << "posible at " << l << endl;
			for (int i=0; i<n; i++) {
				cout << "\tf_" << i << ": ";
				for (auto &ii : functions[i])
					cout << ii << "  ";
				cout << endl;
			}
		}
		// else
		// 	cout << l << endl;
		l++;

		functions.clear();
		// break;
	}


	return 0;
}