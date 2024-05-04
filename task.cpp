#include "task.hpp"
#include<algorithm>

Task::Task(Path& path, std::vector<unsigned> cities_left)
    : cities_left(cities_left)
{
    current_path = &path;
}

Task::Task()
{
}

Task::Task(const Task& task)
{
    if (this != &task)
    {
        current_path = new Path(*task.current_path);
        // Copy vector
        copy(task.cities_left.begin(), task.cities_left.end(), back_inserter(cities_left));
    }
}

Task::~Task() {
    delete current_path;
}

Path& Task::get_current_path()
{
    return *current_path;
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

unsigned Task::extract_city_to_visit(unsigned city)
{
    unsigned return_city = cities_left[city];
    cities_left.erase(cities_left.begin() + city);
    return return_city;
}

__uint128_t Task::add_city_to_path(unsigned city)
{
    return current_path->add(city);
}