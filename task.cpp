#include "task.hpp"

Task::Task(unsigned **weights_matrix, std::vector<unsigned> cities_left)
    : current_path(Path(weights_matrix)), cities_left(cities_left)
{
}

Path &Task::get_current_path()
{
    return current_path;
}

unsigned Task::get_cities_left()
{
    return cities_left.size();
}

unsigned Task::extract_next_city_to_visit()
{
    unsigned city = cities_left.back();
    cities_left.pop_back();
    return city;
}

unsigned Task::add_city_to_path(unsigned city)
{
    return current_path.add(city);
}