/*
Author:
	Ignacio Osorio W.
	
Definition for a ADT graph.
	- Connected or disconnected
	- Simple (no parallel edges or self loops)
	- Undirected graph
	- Not weighted

Algorithms implemented:
	- BFS
	- DFS

	Clique finding algorithm:
	***
	Bron–Kerbosch algorithm, what container should i use?? For fast union and intersect
		*** listing maximal cliques can be set as a SAT problem, reference: https://sci-hub.tw/https://www.researchgate.net/publication/301777400_SAT-based_algorithm_for_finding_all_maximal_cliques

	- Bron_Kerbosch (normal implementation, without optimizations)
		reference: https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm#Without_pivoting
	- WORKING Bron_Kerbosh_pivoting (first optimization)
		reference: https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm#With_pivoting

	- Compact, using the Bron Kerbosch as algorithm to find the maximal cliques
*/

#ifndef MY_GRAPH
#define MY_GRAPH

#include <list>
#include <unordered_map>
#include <set>
#include <utility>
#include <algorithm>
#include <queue>
#include <stack>
#include <string>
#include <vector>

template <typename K=std::string>
class MyGraph {
	public:
		typedef K		VertexId;

		typedef std::set<VertexId>								AdjacencySet;
		typedef std::unordered_map<VertexId, AdjacencySet>		Container;
		typedef std::unordered_map<VertexId,bool>				VertexFlagContainer;

		typedef typename Container::const_iterator				VertexIterator;
		typedef typename AdjacencySet::const_iterator			EdgeIterator;

	protected:
		Container vertex;
		size_t edge_count;

	public:
		MyGraph();
		~MyGraph();

		size_t degree(VertexId v);

		// get_radius();
		// get_center();
		// get_diameter();

		// vertex size
		size_t order();

		// edges size
		size_t size();

		void add_edge(VertexId u, VertexId v);
		void remove_edge(VertexId u, VertexId v);
		
		void add_vertex(VertexId v);
		void remove_vertex(VertexId v);

		VertexIterator vertex_begin();
		VertexIterator vertex_end();

		EdgeIterator Nv_begin(VertexId v);
		EdgeIterator Nv_end(VertexId v);

		std::vector<AdjacencySet> listing_cliques();

		// Searching algorithms
		void bfs(VertexId s, VertexFlagContainer& color=VertexFlagContainer());
		void dfs(VertexId s, VertexFlagContainer& color=VertexFlagContainer());

		void is_connected();

		// Maximal clique finding algorithms 
		template <class Reporter>
		void bron_kerbosch(AdjacencySet R, AdjacencySet P, AdjacencySet X, Reporter report);

		// WORKING HERE!
		// template <class Reporter>
		// void bron_kerbosch_pivoting(AdjacencySet R, AdjacencySet P, AdjacencySet X, Reporter report);
		
		// Compact algorithm using maximal clique listing
		template <class NamingFunc>
		MyGraph<K> compact(NamingFunc naming_function);

		// Complement of the graph
		template<typename T=MyGraph<K>> T complement();

		// Transitive closure
		template<typename T=MyGraph<K>> T transitive_closure();
};

// For linking error purposes
#include "MyGraph.cpp"

#endif