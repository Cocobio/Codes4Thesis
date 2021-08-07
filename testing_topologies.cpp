#include <iostream>
#include "VCDimension.hpp"
#include "Topologias.hpp"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
	string vec;
	int n;
	vector<string> vectores;

	// Lectura de datos
	cin >> n;	
	while(cin >> vec) {
		vectores.push_back(vec);
	}

	// Agregamos los vectores de puros 0 y de puros 1
	sort(vectores.begin(), vectores.end());
	if (vectores.front() != string(n,'0')) vectores.push_back(string(n,'0'));
	if (vectores.back() != string(n,'1')) vectores.push_back(string(n,'1'));

	cout << "expanding..." << endl;
	auto expanded = expandToTopology<>(vectores.begin(), vectores.end());
	sort(expanded.begin(), expanded.end());

	cout << "input expanded, with size " << expanded.size() << endl;
	for (auto &v : expanded)
		cout << v << endl;

	// cout << corolarioSauer(5, 7) << endl;

	return 0;
}