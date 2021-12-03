#include "MyDigraph.hpp"

template<typename K> void MyDigraph<K>::add_edge(VertexId u, VertexId v) {

	auto u_vertex = this->vertex.find(u);
	auto v_vertex = this->vertex.find(v);

	// Check if the vertex are on the graph
	if (u_vertex == this->vertex.end()) {
		this->add_vertex(u);
		u_vertex = this->vertex.find(u);
	}
	if (v_vertex == this->vertex.end()) {
		this->add_vertex(v);
		v_vertex = this->vertex.find(v);
	}

	// Create edge if not presented
	auto tmp = find(u_vertex->second.begin(), u_vertex->second.end(), v);
	if (tmp == u_vertex->second.end()) {
		u_vertex->second.insert(v);

		this->edge_count++;
	}
}

// Remove the edge using 2 vertex. If vertex u == v, it just returns
template<typename K> void MyDigraph<K>::remove_edge(VertexId u, VertexId v) {
	auto u_vertex = this->vertex.find(u);
	auto v_vertex = this->vertex.find(v);

	// If one of the vertex is not on the container, then just return
	if (u_vertex == this->vertex.end() || v_vertex == this->vertex.end())
		return;

	// Find vertex on the neighborhood
	auto v_in_u = find(u_vertex->second.begin(), u_vertex->second.end(), v);

	// Delete the edge
	if (v_in_u != u_vertex->second.end()) {
		u_vertex->second.erase(v_in_u);
		this->edge_count--;
	}
}

// Removes vertex from the graph and all the edges containing it
template<typename K> void MyDigraph<K>::remove_vertex(VertexId v) {
	auto v_vertex = this->vertex.find(v);

	if (v_vertex != this->vertex.end()) {
		this->edge_count-v_vertex->second.size();

		for (auto u_vertex = this->vertex.begin(); u_vertex != this->vertex.end(); u_vertex++) {
			auto it = find(u_vertex->second.begin(), u_vertex->second.end(), v);
			if (u_vertex != v_vertex and it != u_vertex->second.end()) {
				u_vertex->second.erase(it);
				this->edge_count--;
			}
		}
		
		// Remove from vertex container
		this->vertex.erase(v);
	}
}

// Returns the complement of the graph
template<typename K> template<typename T> T MyDigraph<K>::complement() {
	T c;
	AdjacencySet U;

	for (auto &v : this->vertex) {
		U.insert(v.first);
		c.add_vertex(v.first);
	}

	for (auto v = c.vertex_begin(); v!=c.vertex_end(); v++)
		for (auto &u : U)
			c.add_edge(v->first,u);
		// v->second = U;

	for (auto &v : this->vertex)
		for (auto &u : v.second)
			c.remove_edge(v.first, u);

	return c;
}

// Returns the transitive closure graph 
template<typename K> template<typename T> T MyDigraph<K>::transitive_closure() {
	T t_c;
	for (auto &v : this->vertex) {
		VertexFlagContainer color;

		this->bfs(v.first,color);

		for(auto &u : color)
			t_c.add_edge(v.first,u.first);
	}

	return t_c;
}