/*
Esta funcion recibe un rango iterable de vectores que pertenencen al espacio booleano de 2^n

Retorna CONTENEDOR con la expansion de los vectores a una TOPOLOGIA
*/

#ifndef TOPOLOGIAS
#define TOPOLOGIAS

#include <vector>
#include <unordered_set>
#include <string>
#include <utility>
#include <stdexcept>

using namespace std;

unsigned int toBinary(string& binString) {
	if (sizeof(unsigned int)*8 < binString.size())
		throw invalid_argument("To many digits. Limit at 32 bits, for now.");

	unsigned int bin = 0;

	for (auto &d : binString) {
		bin = bin<<1;
		if (d=='0') {}
		else if (d=='1')
			bin += 1;
		else {
			cout << "binary not recognized: " << d << endl;
			throw invalid_argument("Binray expression with unexpected character.");
		}
	}

	return bin;
}

template<class It, class Container=vector<string>>
Container expandToTopology(It begin, It end) {
	unordered_set<unsigned int> T, news, toInsert;

	for (auto i = begin; i!=end; i++)
		T.insert(toBinary(*i));

	for (auto itr_i = T.begin(); itr_i!=T.end(); itr_i++)
		for (auto itr_j = itr_i; itr_j!=T.end(); itr_j++) {   // desde itr_j = itr_i+1
			toInsert.insert((*itr_i)&(*itr_j));
			toInsert.insert((*itr_i)|(*itr_j));
		}

	for (auto &vec : toInsert) T.insert(vec);
	swap(news,toInsert);

	while (news.size() != 0) {
		for (auto &vec : T) {
			for (auto &n : news) {
				unsigned int un = vec|n,
							 in = vec&n;
				if (T.find(un)==T.end()) toInsert.insert(un);
				if (T.find(in)==T.end()) toInsert.insert(in);
			}
		}

		news.clear();

		for (auto &vec : toInsert) T.insert(vec);
		swap(news,toInsert);
	}

	Container TExpanded;
	int n = (*begin).size();

	for (auto bin : T) {
		string stringBin = string(n,'0');

		for (int i=n-1; i>=0; i--) {
			stringBin[i] = (bin&1==1) + '0';//string(bin&1==1);
			bin = bin>>1;
		}
		
		TExpanded.push_back(stringBin);
	}

	return TExpanded;
}

#endif
