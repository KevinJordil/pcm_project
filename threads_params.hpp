#ifndef _thread_params_hpp
#define _thread_params_hpp

#include "path.hpp"
#include "graph.hpp"

#include <atomic>

class ThreadParams
{
public:
    ThreadParams(Graph graph);

    /**
     * @brief Get the paths left
     *
     * @return uint64_t
     */
    uint64_t get_paths_left();

    /**
     * @brief Get the shortest path
     *
     * @return Path
     */
    Path get_shortest_path();

    /**
     * @brief Update the shortest path
     *
     * @param path Path to update
     */
    void update_shortest_path(Path &path);

    /**
     * @brief Decrement the paths left
     *
     * @param paths Number of paths to decrement
     */
    void decrement_paths_left(uint64_t paths);

private:
    Graph graph;
    std::atomic<uint64_t *> paths_left;
    std::atomic<Path *> shortest_path;
    const uint64_t TOTAL_PATHS;
};

#endif //_thread_params_hpp