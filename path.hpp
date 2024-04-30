#ifndef _path_hpp
#define _path_hpp

#include <vector>

#include "graph.hpp"

class Path
{
public:
    Path(Graph graph);

    /**
     * @brief Print the path
     *
     */
    void print();

    /**
     * @brief Add a city to the path
     *
     * @param city City to add
     * @return new path weight
     */
    unsigned add(unsigned city);

    /**
     * @brief Get the path weight
     *
     * @return unsigned
     */
    unsigned get_weight() const;

    /**
     * @brief Get the path
     *
     * @return std::vector<unsigned>
     */
    std::vector<unsigned> get_path();

    /**
     * @brief Get the first city in the path
     *
     * @return unsigned
     */
    unsigned get_first_city();

private:
    Graph graph;
    unsigned path_weight;
    std::vector<unsigned> current_path;
};

#endif //_path_hpp
