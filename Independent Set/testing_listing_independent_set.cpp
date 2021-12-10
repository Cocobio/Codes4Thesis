#include "MyDigraph.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

list<pair<string,list<string>>> parse_input(istream &inputStream);

template <typename T> void cout_graph(T &graph);

template <typename K> void cout_cliques(vector<K> &cliques);

template <typename T> set<set<T>> expand_MIS(vector<set<T>> &mis);

int main(int argc, char *argv[]) {

	if (argc < 2)
		return -1;

	string file = argv[1];

	MyDigraph<string> graph;
	list<pair<string,list<string>>> data;

	ifstream bn_file(file);

	if (!bn_file.is_open())
		return -1;

	data = parse_input(bn_file);
	bn_file.close();

	// Data to graph
	for (pair<string,list<string>> &p : data) {
		string &v = p.first;
			for (string &u : p.second) 
				graph.add_edge(u,v);
	}

	MyDigraph<string> graph_closure = graph.transitive_closure();
	auto graph_complement = graph_closure.complement<MyGraph<string>>();

	vector<set<string>> independent_sets = graph_complement.listing_cliques();


	cout << "Lista de independent sets en el complemento de la clausura transitiva" << endl;
	cout_cliques(independent_sets);

	set<set<string>> subsets = expand_MIS<string>(independent_sets);

	cout << subsets.size() << endl;

	vector<string> o;
	for (auto &is : subsets) {
		string a = "";
		// cout << "{ ";
		a += "{ ";
		for (auto &ith : is)
			// cout << ith << ' ';
			a += ith + " ";
		// cout << "}" << endl;
		a += "}";
		o.push_back(a);
	}

	sort(o.begin(), o.end(), [](string &a, string &b){ return a.size()<b.size() or (a.size()==b.size() and a<b); });

	for (auto &is : o)
		cout << is << endl;

	return 0;
}


list<pair<string,list<string>>> parse_input(istream &inputStream) {
	list<pair<string,list<string>>> g;
	string tmp_str;

	while (getline(inputStream,tmp_str)) {
		stringstream s(tmp_str);

		string v, u;
		list<string> in_degree_v;
		s >> v;

		while (s >> u)
			if (u != "=" and u != "&")
				in_degree_v.push_back(u);

		g.emplace_back(v,in_degree_v);
	}

	return g;
}



template <typename T> void cout_graph(T &graph) {

	for (auto v=graph.vertex_begin(); v!=graph.vertex_end(); v++) {
		cout << (v->first) << "\t:";
		for (auto &u : v->second)
			cout << u << "  ";
		cout << endl;
	}
}

template <typename K> void cout_cliques(vector<K> &cliques) {
	for (K &clique : cliques) {
		cout << "{ ";
		for (auto v : clique)
			cout << v << " ";
		cout << "}" << endl;
	}
}



template <typename T> set<set<T>> expand_MIS(vector<set<T>> &mis){
	set<set<T>> subsets;
	subsets.insert(set<T>());

	for(int i=0; i<mis.size(); i++) {
		string mask = string(mis[i].size(),'0');
		
		for (int j=mis[i].size()-1; j>=0; j--) {
			mask[j] = '1';

			do {
				set<T> tmp;
				for (int k=0; k<mask.size(); k++)
					if (mask[k]=='1') {
						set<T>::iterator it = mis[i].begin();
						advance(it, k);
						tmp.insert(*it);
					}
				subsets.insert(tmp);
			} while(next_permutation(mask.begin(),mask.end()));
		}
		
	}

	return subsets;
}