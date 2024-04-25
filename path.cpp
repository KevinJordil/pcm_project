#include "path.hpp"

#include <iostream>

Path::Path(unsigned **weights_matrix) : weights_matrix(weights_matrix), path_weight(0)
{
}

void Path::print()
{
    for (auto city : current_path)
    {
        std::cout << city << " ";
    }
    std::cout << std::endl;
}

unsigned Path::add(unsigned city)
{
    current_path.push_back(city);
    if (current_path.size() > 1)
    {
        path_weight += weights_matrix[current_path[current_path.size() - 2]][city];
    }
    return path_weight;
}

unsigned Path::get_weight()
{
    return path_weight;
}

std::vector<unsigned> Path::get_path()
{
    return current_path;
}

unsigned Path::get_first_city()
{
    return current_path[0];
}