//
//  path.hpp
//  
//  Copyright (c) 2022 Marcelo Pasin. All rights reserved.
//


#ifndef _path_hpp
#define _path_hpp

#include "graph.hpp"
#include <iostream>
#include <bitset>
#include "settings.hpp"

class Path {
private:
	size_t _size;
	dist_t _distance;
	node_t _nodes[TSP_MAX_NODES]; // OPTI: nodes are stored statically in a Path
	std::bitset<32> _contained_nodes;
	Graph* _graph;

public:
	Path() : _size(0), _distance(UINT64_MAX), _nodes{}, _contained_nodes(), _graph(nullptr) {}

	Path(Graph* graph) : _size(0), _distance(0), _nodes{}, _contained_nodes(), _graph(graph) {}

	Path(Path const& o) {
		*this = o;
	}

	Path& operator=(Path const& o) {
		if (this != &o) {
			_graph = o._graph;
			_size = o._size;
			_distance = o._distance;
			_contained_nodes = o._contained_nodes;

			for (size_t i = 0; i < _size; i++)
				_nodes[i] = o._nodes[i];
		}

		return *this;
	}

	size_t max() const { return _graph->size(); }
	size_t size() const { return _size; }
	size_t missing() const { return max() - _size; }
	bool leaf() const { return (_size == max()); }
	dist_t distance() const { return _distance; }
	dist_t forecast_distance() const { return _distance + _graph->min_distance(missing()); }



	void add(node_t node) {
		if (_size <= max()) {
			if (_size) {
				node_t last = _nodes[_size - 1];
				dist_t distance = _graph->distance(last, node);
				_distance += distance;
			}

			_nodes[_size++] = node;
			_contained_nodes.set(node);
		}
	}

	void pop() {
		if (_size) {
			node_t last = _nodes[--_size];
			_contained_nodes.reset(last);

			if (_size) {
				node_t node = _nodes[_size - 1];
				dist_t distance = _graph->distance(node, last);
				_distance -= distance;
			}
		}
	}

	bool contains(node_t node) const {
		return _contained_nodes[node];
	}

	void print(std::ostream& os) const {
		os << '[' << _distance;
		for (size_t i = 0; i < _size; i++)
			os << (i ? ',' : ':') << ' ' << (unsigned)_nodes[i];
		os << ']';
	}
};

inline std::ostream& operator <<(std::ostream& os, Path* p) {
	p->print(os);
	return os;
}

#endif // _path_hpp