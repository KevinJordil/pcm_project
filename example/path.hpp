//
//  path.hpp
//  
//  Copyright (c) 2022 Marcelo Pasin. All rights reserved.
//

#include <iostream>

#ifndef _path_hpp
#define _path_hpp

#include "graph.hpp"

class Path {
private:
	int size;
	int distance;
	int* _nodes;
	Graph* graph;
public:
	~Path()
	{
		clear();
		delete[] _nodes;
		_nodes = 0;
		graph = 0;
	}

	Path(Graph* graph)
	{
		graph = graph;
		_nodes = new int[max() + 1];
		distance = 0;
		clear();
	}

	int max() const { return graph->size(); }
	int size() const { return size; }
	bool leaf() const { return (size == max()); }
	int distance() const { return distance; }
	void clear() { size = distance = 0; }

	void add(int node)
	{
		if (size <= max()) {
			if (size) {
				int last = _nodes[size - 1];
				int distance = graph->distance(last, node);
				distance += distance;
			}
			_nodes[size++] = node;
		}
	}

	void pop()
	{
		if (size) {
			int last = _nodes[--size];
			if (size) {
				int node = _nodes[size - 1];
				int distance = graph->distance(node, last);
				distance -= distance;
			}
		}
	}

	bool contains(int node) const
	{
		for (int i = 0; i < size; i++)
			if (_nodes[i] == node)
				return true;
		return false;
	}

	void copy(Path* o)
	{
		if (max() != o->max()) {
			delete[] _nodes;
			_nodes = new int[o->max() + 1];
		}
		graph = o->graph;
		size = o->size;
		distance = o->distance;
		for (int i = 0; i < size; i++)
			_nodes[i] = o->_nodes[i];
	}

	void print(std::ostream& os) const
	{
		os << '[' << distance;
		for (int i = 0; i < size; i++)
			os << (i ? ',' : ':') << ' ' << _nodes[i];
		os << ']';
	}
};

std::ostream& operator <<(std::ostream& os, Path* p)
{
	p->print(os);
	return os;
}

#endif // _path_hpp