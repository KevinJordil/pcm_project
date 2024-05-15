#ifndef _thread_params_hpp
#define _thread_params_hpp

#include "path.hpp"
#include "graph.hpp"
#include "concurrentqueue.hpp"

#include <atomic>

class ThreadParams
{
public:
    ThreadParams(Graph* graph, moodycamel::ConcurrentQueue<Path*>* queue);

    /**
     * @brief Get the paths left
     *
     * @return size_t
     */
    size_t get_paths_left();

    /**
     * @brief Get the shortest path
     *
     * @return Path
     */
    Path* get_shortest_path();

    /**
     * @brief Set the shortest path
     *
     * @param path Path to update
     */
    void set_shortest_path(Path* path);

    /**
     * @brief Decrement the paths left
     *
     * @param paths Number of paths to decrement
     */
    void decrement_paths_left(size_t paths);

    /**
     * @brief Get the next path object to explore
     *
     * @return Path
     */
    Path* fetch_branch();

    /**
     * @brief Add a path to the queue
     *
     * @param path Path to adds
     */
    void publish_branch(Path* path);

    /**
    * @brief Get the weight of the shortest path
    *
    * @return dist_t
    */
    dist_t shortest_distance();

    /**
    * @brief Get the size of the graph
    *
    * @return size_t
    */
    size_t number_of_nodes();

private:
    Graph* graph;
    std::atomic<size_t> paths_left;
    std::atomic<Path*> shortest_path;
    moodycamel::ConcurrentQueue<Path*>* queue;
    const size_t TOTAL_PATHS;
};

#endif //_thread_params_hpp