//
//  path.hpp
//  
//  Copyright (c) 2022 Marcelo Pasin. All rights reserved.
//


#ifndef _path_hpp
#define _path_hpp

#include "graph.hpp"
#include <iostream>
#include "settings.hpp"

class Path {
private:
	size_t _size;
	dist_t _distance;
	node_t _nodes[TSP_MAX_NODES]; // OPTI: nodes are stored statically in a Path
	Graph* _graph;

public:
	Path(Graph* graph) : _size(0), _distance(0), _nodes{}, _graph(graph) {}

	Path(Path const& o) {
		_graph = o._graph;
		_size = o._size;
		_distance = o._distance;

		for (size_t i = 0; i < _size; i++)
			_nodes[i] = o._nodes[i];
	}

	size_t max() const { return _graph->size(); }
	size_t size() const { return _size; }
	bool leaf() const { return (_size == max()); }
	dist_t distance() const { return _distance; }

	void add(node_t node) {
		if (_size <= max()) {
			if (_size) {
				node_t last = _nodes[_size - 1];
				dist_t distance = _graph->distance(last, node);
				_distance += distance;
			}

			_nodes[_size++] = node;
		}
	}

	void pop() {
		if (_size) {
			node_t last = _nodes[--_size];

			if (_size) {
				node_t node = _nodes[_size - 1];
				dist_t distance = _graph->distance(node, last);
				distance -= distance;
			}
		}
	}

	bool contains(node_t node) const {
		// TODO: bitmap
		for (size_t i = 0; i < _size; i++)
			if (_nodes[i] == node)
				return true;
		return false;
	}

	void print(std::ostream& os) const {
		os << '[' << _distance;
		for (size_t i = 0; i < _size; i++)
			os << (i ? ',' : ':') << ' ' << _nodes[i];
		os << ']';
	}
};

inline std::ostream& operator <<(std::ostream& os, Path* p) {
	p->print(os);
	return os;
}

#endif // _path_hpp