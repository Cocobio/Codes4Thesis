#include "MyGraph.hpp"

// Constructor
template<typename K> MyGraph<K>::MyGraph() {
	
}

// Destructor
template<typename K> MyGraph<K>::~MyGraph() {
	
}

// Order of the graph (defined as the cardinal of vertex)
template<typename K> size_t MyGraph<K>::order() {
	return vertex.size();
}

// Size of the graph (defined as the cardinal of the edges)
// can also be computed as 1/2*sum(for_each v in vertex, degree(v))
template<typename K> size_t MyGraph<K>::size() {
	return edge_count;
}

// Chech if the graph is connected or disconnected
template<typename K> void MyGraph<K>::is_connected() {
	VertexId v = vertex.begin()->first;
	VertexFlagContainer color;

	bfs(v, color);

	return color.size()==vertex.size();
}

// Breast First Search algorithm
template<typename K> void MyGraph<K>::bfs(VertexId s, VertexFlagContainer& color) {
	std::queue<VertexId> q;

	q.push(s);

	while (!q.empty()) {
		VertexId current = q.front();
		q.pop();

		if (!color[current]) {
			color[current] = true;

			for (auto &v : vertex.find(current)->second)
				q.push(v);
		}
	}
}

// Depth First Search algorithm
template<typename K> void MyGraph<K>::dfs(VertexId s, VertexFlagContainer& color) {
	std::stack<VertexId> q;

	q.push(s);

	while (!q.empty()) {
		VertexId current = q.top();
		q.pop();

		if(!color[current]) {
			color[current] = true;

			for (auto &v : vertex.find(current)->second)
				q.push(v);
		}
	}
}

// Get the degree of a vertex v
template<typename K> size_t MyGraph<K>::degree(VertexId v) {
	auto v_data = vertex.find(v);

	if (v_data == vertex.end()) return 0;
	else return v_data->second.size();
}


// Adding an edge, if one of the vertex doesn't exist it will create the vertex
// The edge will be created only if the edge doesn't exist
template<typename K> void MyGraph<K>::add_edge(VertexId u, VertexId v) {

	auto u_vertex = vertex.find(u);
	auto v_vertex = vertex.find(v);

	// Check if the vertex are on the graph
	if (u_vertex == vertex.end()) {
		add_vertex(u);
		u_vertex = vertex.find(u);
	}
	if (v_vertex == vertex.end()) {
		add_vertex(v);
		v_vertex = vertex.find(v);
	}

	// Create edge if not presented and its not a self loop
	auto tmp = find(v_vertex->second.begin(), v_vertex->second.end(), u);
	if (tmp == v_vertex->second.end() && u!=v) {
		v_vertex->second.insert(u);
		u_vertex->second.insert(v);

		edge_count++;
	}
}

// Remove the edge using 2 vertex. If vertex u == v, it just returns
template<typename K> void MyGraph<K>::remove_edge(VertexId u, VertexId v) {
	auto u_vertex = vertex.find(u);
	auto v_vertex = vertex.find(v);

	// If one of the vertex is not on the container, then just return
	// or return if it tryies to remove a self edge, which should not exist
	if (u_vertex == vertex.end() || v_vertex == vertex.end() || u==v)
		return;

	// Find each each vertex on the neighborhood of the other vertex
	auto u_in_v = find(v_vertex->second.begin(), v_vertex->second.end(), u);
	auto v_in_u = find(u_vertex->second.begin(), u_vertex->second.end(), v);

	// Delete the edge
	v_vertex->second.erase(u_in_v);
	u_vertex->second.erase(v_in_u);

	edge_count--;
}

// Creates a new vertex only if its not already on the graph
template<typename K> void MyGraph<K>::add_vertex(VertexId v) {
	if (vertex.find(v) == vertex.end())
		vertex.emplace(v, AdjacencySet());
}

// Removes vertex from the graph and all the edges containing it
template<typename K> void MyGraph<K>::remove_vertex(VertexId v) {
	auto v_vertex = vertex.find(v);

	if (v_vertex != vertex.end()) {
		edge_count-v_vertex->second.size();
		
		// Remove edges that are contained
		for (auto &Nv_i : v_vertex->second) {
			AdjacencySet *list_ref = &vertex.find(Nv_i)->second;
			auto it = find(list_ref->begin(), list_ref->end(), v);
			list_ref->erase(it);
		}

		// Remove from vertex container
		vertex.erase(v);
	}
}

// Wrapper function for the maximal clique finding algorithms (bron kerbosch)
// It returns a vector with sets containing the vertex of maximals clique.
// The vertex is ordered on descending size of clique (or kth degree)
template<typename K> std::vector<typename MyGraph<K>::AdjacencySet> MyGraph<K>::listing_cliques() {
	std::vector<AdjacencySet> cliques;

	// Parameters for bron_kerbosch. Initialize the P set
	AdjacencySet R, P, X;
	for (auto &v : vertex) P.insert(v.first);

	bron_kerbosch(R, P, X, [&cliques](AdjacencySet clique) { cliques.push_back(clique); });

	// Sort the maximals cliques on descending order, using their kth grade (or size) 
	std::sort(cliques.begin(), cliques.end(), [] (AdjacencySet a, AdjacencySet b) { return a.size() > b.size(); });

	return cliques;
}

// Finds all the maximal cliques and uses the report parameter to report the found clique.
template<typename K> template<class Reporter> void MyGraph<K>::bron_kerbosch(AdjacencySet R, AdjacencySet P, AdjacencySet X, Reporter report) {
	if (P.empty() && X.empty()) report(R);

	AdjacencySet tmpP(P);

	for (auto &v : tmpP) {
		// R union {v}
		R.insert(v);

		// Neighborhood of v
		AdjacencySet* Nv = &vertex[v];

		// P intersection with N(v)
		AdjacencySet P_intersect_Nv;
		std::set_intersection(P.begin(), P.end(), Nv->begin(), Nv->end(), std::inserter(P_intersect_Nv, P_intersect_Nv.begin())); 

		// X intersection with N(v)
		AdjacencySet X_intersect_Nv;
		std::set_intersection(X.begin(), X.end(), Nv->begin(), Nv->end(), std::inserter(X_intersect_Nv, X_intersect_Nv.begin())); 

		bron_kerbosch(R,P_intersect_Nv,X_intersect_Nv,report);
		
		// Clean v from R, move v from P to X
		R.erase(v);
		X.insert(v);
		P.erase(v);
	}
}

// Compact funciton, It will use a lookup table to asociate the vertex with it self or a maximal clique component
template<typename K> template<class NamingFunc> MyGraph<K> MyGraph<K>::compact(NamingFunc naming_function) {
	MyGraph<K> compact_graph;


	struct CustomCompare {
		bool operator()(const AdjacencySet& lhs, const AdjacencySet& rhs)
		{
			return lhs.size() < rhs.size();
		}
	};

	std::vector<AdjacencySet> cliques = listing_cliques();
	std::priority_queue<AdjacencySet, std::vector<AdjacencySet>, CustomCompare> maximals_cliques(cliques.begin(), cliques.end());

	std::unordered_map<VertexId, VertexId> components;

	// Counter for identifying different maximal cliques
	size_t component_i = 0;

	// Compact cliques from the biggest to the smalles, removing vertex shared by cliques
	while (maximals_cliques.size()) {
		AdjacencySet clique = maximals_cliques.top();
		bool clique_not_assigned = true;
		maximals_cliques.pop();
		
		if (clique.size() <= 2) break;

		std::list<VertexId> erase_ids;

		// Check that all the vertex on this particular clique hasn't been collapsed (compacted) already
		for (auto &v : clique)
			if (components.find(v) != components.end()) {
				clique_not_assigned = false;
				erase_ids.push_back(v);
			}

		// If this clique has all its vertex available for compact, then mark then as part of the new collapse component
		if (clique_not_assigned) {
			VertexId component_id = naming_function(component_i);

			for (auto &v : clique)
				components[v] = component_id;
			component_i++;
		}
		else {
			for (auto &v : erase_ids)
				clique.erase(v);
			erase_ids.clear();

			if (clique.size() > 2)
				maximals_cliques.push(clique);
		}
	}

	// Mark the rest of the vertex as individuals, not part of a compacted clique
	for (auto &v : vertex)
		if (components.find(v.first) == components.end())
			components[v.first] = v.first;

	// Add connections to the compact graph
	for (auto &v : vertex)
		for (auto &Nv_i : v.second)
			compact_graph.add_edge(components[v.first], components[Nv_i]);
	
	return compact_graph;
}


// Returns the complement of the graph
template<typename K> template<typename T> T MyGraph<K>::complement() {
	T c;
	AdjacencySet U;

	for (auto &v : vertex) {
		U.insert(v.first);
		c.add_vertex(v.first);
	}

	for (auto &v : c.vertex)
		v.second = U;

	for (auto &v : vertex)
		for (auto &u : v.second)
			c.remove_edge(v.first, u);

	return c;
}

// Returns the transitive closure graph 
template<typename K> template<typename T> T MyGraph<K>::transitive_closure() {
	T t_c;
	for (auto &v : vertex) {
		VertexFlagContainer color;

		bfs(v.first,color);

		for(auto &u : color)
			t_c.add_edge(v.first,u.first);
	}

	return t_c;
}



///////////////////////////// Const Iterators accesors and limiters /////////////////////////////
template<typename K> typename MyGraph<K>::VertexIterator MyGraph<K>::vertex_begin() {
	return vertex.begin();
}

template<typename K> typename MyGraph<K>::VertexIterator MyGraph<K>::vertex_end() {
	return vertex.end();
}

template<typename K> typename MyGraph<K>::EdgeIterator MyGraph<K>::Nv_begin(VertexId v) {
	return vertex.find(v)->second.cbegin();
}

template<typename K> typename MyGraph<K>::EdgeIterator MyGraph<K>::Nv_end(VertexId v) {
	return vertex.find(v)->second.cend();	
}

