#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <unordered_set>
#include <math.h>

using namespace std;

int binom(int n, int k);

int corolario_Sauer(int n, int cardinal_F);
bool shatter(vector<string> &F, string &mask);
vector<short> get_index(string &mask);

int main() {
	size_t n;
	vector<string> vectors;
	
	string str;
	unordered_set<string> unique_vecs;

	// Input
	cin >> n;
	string mask(n,'0');

	while (cin >> str) {
		if (str.size()!=n)
			throw runtime_error("Vectors must by in {0,1}^n space. (Wrong size)");
		for (auto &b : str)
			if (b!='0' and b!='1')
				throw runtime_error("Vectors must by in {0,1}^n space. (Character differnt than 0 or 1)");
	
		unique_vecs.insert(str);
	}
	vectors = vector<string>(unique_vecs.begin(), unique_vecs.end());
	unique_vecs.clear();

	// VC-Dim
	int vc = 0;
	vector<vector<short>> Ds;

	int min = corolario_Sauer(n, unique_vecs.size());
	int max = floor(log2(unique_vecs.size()));

	for (int i=1; i<=min; i++) {
		mask[mask.size()-i] = '1';
	}

	for (int i=min; i<=max; i++) {
		mask[mask.size()-i] = '1';

		do {
			if (shatter(vectors,mask)) {
				if (Ds.size() and vc!=i) {
					vc = i;
					Ds.clear();
				}
				Ds.push_back(get_index(mask));
			}
			else if (vc+2==i)
				break;

		} while (next_permutation(mask.begin(),mask.end()));

	}

	// Output results
	if (Ds.size()!=0) {
		cout << "VC-dimension: " << Ds[0].size() << endl;
		cout << "Sets:" << endl;

		// Output sets
		for (auto &D : Ds) {
			cout << "\t{ ";
			for (auto &idx : D)
				cout << idx << " ";
		cout << "}" << endl;
		}
	}
	else {
		cout << "VC-dimension: 0" << endl;
	}

	return 0;
}

int corolario_Sauer(int n, int cardinal_F) {
	int k = 0;
	int sum = 0;

	while (sum < cardinal_F)
		sum += binom(n,k++);

	return k;
}

bool shatter(vector<string> &F, string &D) {
	unordered_set<string> combinations;

	int dim = 0;

	for (auto &c : D)
		if (c=='1') dim++;
		
	// Family must contain at least 2^dim vectors
	if (F.size()<(1<<dim))
		return false;

	for (auto &vec : F) {
		string str(D.size(), '0');
		for (int i=0; i<D.size(); i++)
			if (D[i]=='1')
				str[i] = vec[i];
		combinations.insert(str);
	}

	return combinations.size()==(1<<dim);
}

vector<short> get_index(string &mask) {
	vector<short> idx;

	for (int i=0; i<mask.size(); i++)
		if (mask[i]=='1')
			idx.push_back(i+1);

	return idx;
}


int binom(int n, int k) {
	if (k==0) return 1;

	int res = 1;	
	// for (int i=1; i<=n; i++)
	// 	res *= i;

	// for (int i=1; i<=k; i++)
	// 	res /= i;
	
	// for (int i=1; i<=n-k; i++)
	// 	res /= i;

	for (int i=k+1; i<=n; i++)
		res *= i;

	for (int i=1; i<=n-k; i++)
		res /= i;

	return res;
}