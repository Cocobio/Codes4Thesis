#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <unordered_set>

using namespace std;

bool shatter(vector<string> &F, string &mask);
vector<short> get_index(string &mask);
int vc_dim(int n, vector<string> &vectors) ;

int max_subsets(int n, int k);

int main() {
	int n = 6;
	int k = 2;
	int subsets = max_subsets(n,k);
	vector<string> all_vectors;

	bool flag = false;

	for (int i=0; i<=n; i++) {
		string vec(n,'0');
		for (int j=0; j<i; j++)
			vec[j] = '1';

		sort(vec.begin(), vec.end());

		do {
			all_vectors.push_back(vec);
		} while (next_permutation(vec.begin(), vec.end()));
	}

	sort(all_vectors.begin(), all_vectors.end());

	cout << "all vectores created." << endl;

	string mask(1<<n, '0');
	for (int i=0; i<subsets-1; i++)
		mask[i] = '1';
	sort(mask.begin(),mask.end());	
	mask[0] = '1';


	do {
		vector<string> vectors;
		for (int i=0; i<mask.size(); i++)
			if (mask[i]=='1')
				vectors.push_back(all_vectors[i]);

		if (vectors[0] == string(n, '0') and vectors[subsets-1] == string(n,'1')) {
			int vc = vc_dim(n,vectors);
			if (vc<=k) {
				flag = true;

				for(auto &s : vectors)
					cout << "\t" << s;
				cout << endl; 
			}
		}

		vectors.clear();

	}	while (next_permutation(mask.begin()+1, mask.end()-1));

	cout << "flag: " << flag << endl;
	cout << subsets << endl;

	return 0;
}

int vc_dim(int n, vector<string> &vectors) {
	string mask(n,'0');

	// VC-Dim
	int vc = 0;
	vector<vector<short>> Ds;

	for(int i=1; i<=n; i++) {
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

		} while(next_permutation(mask.begin(),mask.end()));

	}

	// Output results
	if (Ds.size()!=0) {
		return Ds[0].size();
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
		return 0;
		cout << "VC-dimension: 0" << endl;
	}

	return 0;
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

int combi(int n, int k) {
	int res = 1;	
	for (int i=1; i<=n; i++)
		res *= i;

	for (int i=1; i<=k; i++)
		res /= i;
	
	for (int i=1; i<=n-k; i++)
		res /= i;

	return res;
}

int max_subsets(int n, int k) {
	int res = 1;	
	for (int i=1; i<=k; i++)
		res += combi(n,i);		

	return res;
}

