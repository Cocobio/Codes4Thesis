#ifndef VCDIMENSION
#define VCDIMENSION

#include <bitset>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <unordered_set>
#include <math.h>
#include <iostream>

using namespace std;

// https://cp-algorithms.com/combinatorics/binomial-coefficients.html
// Calculo del espacio binomian de n sobre k
int binom(int n, int k) {
    double res = 1;
    for (int i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return (int)(res + 0.01);
}

// Devuelve el maximo k que cumple con el corolario de Sauer para la VC-dim y 
// el cardinal del conjunto. Este corolario se describe en el informe.
// Basicamente devuelve el minimo k (VC-Dim) dado solo el cardinal de un conjunto.
// \sum_{i=0}^{k} \binom{n}{i} < |F|
int corolarioSauer(int n, int cardinalF) {
	int k = 0;
	int sum = 0;

	while (sum < cardinalF)
		sum += binom(n,k++);

	return k-1;
}

int sumatoriaSauer(int n, int k) {
	int sum = 0;

	for (int i=0; i<=k; i++)
		sum += binom(n,i);

	return sum;
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

void uniqueVecs(vector<string> &S) {	
	unordered_set<string> unique_vecs;

	for (string &v : S)
		unique_vecs.insert(v);

	if (unique_vecs.size() < S.size()) {
		S.clear();
		S = vector<string>(unique_vecs.begin(), unique_vecs.end());
	}
}


// Devuelve los indices que forman la VC dimension
vector<short> vc_dim(vector<string> &S) {
	// Por Sauer y cardinal tendremos un lower y un upper bound para la VC-dimension
	int n = S[0].size();
	int vc_lb = corolarioSauer(n, S.size());
	int vc_ub = floor(log2(S.size()));

	// indices de la VC-dim
	vector<short> idx;

	// Creamos una mascara para probar los indices que fragmentan S
	string mask(n,'0');
	for (int i=1; i<=vc_lb; i++) {
		mask[mask.size()-i] = '1';
	}

	// Probamos todas las posibilidades entre el lower bound y el upper bound
	for (int i=vc_lb; i<=vc_ub; i++) {
		do {
			if (shatter(S,mask)) {
				idx = get_index(mask);
				sort(mask.begin(), mask.end());
				break;

				// if (Ds.size() and vc!=i) {
				// 	vc = i;
				// 	Ds.clear();
				// }
				// Ds.push_back(get_index(mask));
			}
			// else if (idx.size()>=+2==i)
			// 	break;

		} while (next_permutation(mask.begin(),mask.end()));

		mask[mask.size()-i-1] = '1';
	}

	if (idx.size()<vc_lb) {
		cout << "Mask: " << mask << endl;
		cout << "vc_lb: " << vc_lb << "\nvc_ub: " << vc_ub << endl;
		cout << "Index found:" << endl;
		for (short &i : idx)
			cout << "\t" << i << endl;
		throw logic_error("Less index found than the lower bound.");
	}

	return idx;
}






int test(vector<string> &VC) {
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

	int min = corolarioSauer(n, vectors.size());
	int max = floor(log2(vectors.size()));

	cout << "min: " << min << "\nmax: " << max << endl;

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
	return Ds[0].size();
	if (Ds.size()!=0) {
		cout << "VC-dimension: " << vc << endl;
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
	}
}







#endif