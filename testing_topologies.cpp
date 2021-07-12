#include <iostream>
#include "VCDimension.hpp"
#include "Topologias.hpp"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
	// string vec;
	// int n;
	// vector<string> vectores;

	// cin >> n;	

	// while(cin >> vec) {
	// 	vectores.push_back(vec);
	// }

	// cout << "expanding..." << endl;
	// auto expanded = expandToTopology<>(vectores.begin(), vectores.end());
	// sort(expanded.begin(), expanded.end());

	// cout << "input expanded, with size " << expanded.size() << endl;
	// for (auto &v : expanded)
	// 	cout << v << endl;

	cout << corolarioSauer(5, 7) << endl;

	return 0;
}