#include "MyDigraph.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

list<pair<string,list<string>>> parse_input(istream &inputStream);

template <typename T> void cout_graph(T &graph);

template <typename K> void cout_cliques(vector<K> &cliques);

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

	vector<set<string>> cliques = graph_complement.listing_cliques();

	// cout
	// cout_graph(graph);
	// cout << endl << endl;
	// cout_graph(graph_closure);
	// cout << endl << endl;
	// cout_graph(graph_complement);

	cout << "Lista de independent sets en el complemento de la clausura transitiva" << endl;
	cout_cliques(cliques);

	set<set<string>> subsets;

	for(int i=0; i<cliques.size(); i++) {
		string mask = string(cliques[i].size(),'0');
		mask[0] = '1';
		sort(mask.begin(),mask.end());
		
		
	}


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
