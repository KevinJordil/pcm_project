//
//  graph.hpp
//
//  Copyright (c) 2024 Marcelo Pasin. All rights reserved.
//

#ifndef _graph_hpp
#define _graph_hpp

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include "settings.hpp"

class Graph
{
private:
	size_t _max_size;
	size_t _size;
	dist_t* _distances;
	dist_t _min_distance;
	size_t* _x;
	size_t* _y;

public:
	Graph(size_t size)
	{
		_max_size = size;
		_distances = new size_t[size * size];
		for (size_t i = 0; i < size; i++)
			for (size_t j = 0; j < size; j++)
				sdistance(i, j) = -1;
		_min_distance = UINT64_MAX;
		_x = new size_t[size];
		_y = new size_t[size];
		_size = 0;
	}

	~Graph()
	{
		delete[] _x;
		delete[] _y;
		delete[] _distances;
		_x = _y = _distances = 0;
		_max_size = 0;
	}

	size_t size() const { return _size; }
	dist_t distance(node_t i, node_t j) const { return _distances[i + _max_size * j]; }
	dist_t& sdistance(node_t i, node_t j) { return _distances[i + _max_size * j]; }
	dist_t min_distance() const { return _min_distance; }
	dist_t& smin_distance() { return _min_distance; }
	size_t add(int x, int y)
	{
		_x[_size] = x;
		_y[_size] = y;
		return _size++;
	}

	void print(std::ostream& os, bool all = true) const
	{
		os << "     ";
		for (size_t i = (all ? 0 : 1); i < _size; i++)
		{
			char fmt[10];
			snprintf(fmt, 8, "%5ld", i);
			os << fmt;
		}
		os << '\n';
		for (size_t i = 0; i < (_size - (all ? 0 : 1)); i++)
		{
			char fmt[10];
			snprintf(fmt, 8, "%5ld", i);
			os << fmt;
			for (size_t j = 0; j < (all ? 0 : i); j++)
				os << "   ..";
			for (size_t j = (all ? 0 : i + 1); j < _size; j++)
			{
				snprintf(fmt, 8, "%5ld", distance(i, j));
				os << fmt;
			}
			os << '\n';
		}
	}
};

#endif // _graph_hpp
