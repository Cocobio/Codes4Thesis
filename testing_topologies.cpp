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
	cout << "input with size " << vectores.size() << endl;
	for (auto &v : vectores)
		cout << v << endl;

	if (vectores.front() != string(n,'0')) vectores.push_back(string(n,'0'));
	if (vectores.back() != string(n,'1')) vectores.push_back(string(n,'1'));

	cout << "expanding..." << endl;
	vector<string> expanded = expandToTopology<>(vectores.begin(), vectores.end());
	sort(expanded.begin(), expanded.end());

	cout << "input expanded, with size " << expanded.size() << endl;
	for (auto &v : expanded)
		cout << v << endl;

	// cout << corolarioSauer(5, 7) << endl;
	vector<short> original = vc_dim(vectores);
	cout << "VC dim input: " << original.size() << endl;
	cout << "idxs: ";
	for (auto &i : original)
		cout << i << " ";
	cout << endl;

	vector<short> topo = vc_dim(expanded);
	cout << "VC dim expanded: " << topo.size() << endl;
	cout << "idxs: ";
	for (auto &i : topo)
		cout << i << " ";
	cout << endl;

	return 0;
}