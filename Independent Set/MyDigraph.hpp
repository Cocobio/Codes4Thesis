#ifndef MY_DIGRAPH
#define MY_DIGRAPH

#include "MyGraph.hpp"

template <typename K=std::string>
class MyDigraph : public MyGraph<K> {
	public:
		typedef K		VertexId;

		typedef std::set<VertexId>								AdjacencySet;
		typedef std::unordered_map<VertexId, AdjacencySet>		Container;
		typedef std::unordered_map<VertexId,bool>				VertexFlagContainer;

		typedef typename Container::const_iterator				VertexIterator;
		typedef typename AdjacencySet::const_iterator			EdgeIterator;

		void add_edge(VertexId u, VertexId v);
		void remove_edge(VertexId u, VertexId v);
		void remove_vertex(VertexId v);

		template<typename T=MyDigraph<K>> T complement();
		template<typename T=MyDigraph<K>> T transitive_closure();
};


#include "MyDigraph.cpp"

#endif