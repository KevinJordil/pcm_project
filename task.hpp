#ifndef _task_hpp
#define _task_hpp

#include <vector>

#include "path.hpp"
#include "graph.hpp"

class Task
{
public:
    Task(Path &path, std::vector<unsigned> cities_left);
    Task();

    /**
    * @brief Copy constructor
    *
    * @param task Task to copy
    */
    Task(const Task &task);

    /**
     * @brief Get the current path
     *
     * @return Path
     */
    Path &get_current_path();

    /**
     * @brief Get the number of cities left
     *
     * @return unsigned
     */
    unsigned get_cities_left();

    /**
     * @brief Extract the next city to visit
     *
     * @return unsigned
     */
    unsigned extract_next_city_to_visit();

    /**
     * @brief Add a city to the path
     *
     * @param unsigned City to add to the path
     * @return unsigned The new path weight
     */
    unsigned add_city_to_path(unsigned city);

private:
    Path *current_path;
    std::vector<unsigned> cities_left;
};

#endif //_task_hpp
