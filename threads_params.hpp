#ifndef _thread_params_hpp
#define _thread_params_hpp

#include "path.hpp"
#include "graph.hpp"
#include "concurrentqueue.hpp"
#include "tools.hpp"

#include <atomic>

template <size_t N>
class ThreadParams {
public:
    ThreadParams(Graph* graph) :
        graph(graph),
        paths_left(factorial[graph->size() - 1]),
        shortest_path(new Path(UINT64_MAX)),
        queues(),
        TOTAL_PATHS(factorial[graph->size() - 1]) {
        if (graph->size() >= TSP_MAX_NODES)
            throw "Graphs must contain less than 20 edges";
    }

    /**
     * @brief Get the paths left
     *
     * @return size_t
     */
    size_t get_paths_left() {
        return paths_left;
    }

    /**
     * @brief Get the shortest path
     *
     * @return Path
     */
    Path* get_shortest_path() {
        return shortest_path;
    }

    /**
     * @brief Set the shortest path
     *
     * @param path Path to update
     */
    void set_shortest_path(Path* path) {
        Path* expected;
        Path* desired;

        do {
            expected = this->shortest_path.load();
            desired = path;

            // Stop trying to update if a better path appeared
            if (expected != nullptr && expected->distance() < desired->distance()) break;
        } while (!this->shortest_path.compare_exchange_weak(expected, desired));
    }

    /**
     * @brief Decrement the paths left
     *
     * @param paths Number of paths to decrement
     */
    void decrement_paths_left(size_t paths) {
        size_t expected;
        size_t desired;

        do {
            expected = this->paths_left.load();
            desired = expected - paths;
        } while (!paths_left.compare_exchange_weak(expected, desired));
    }

    /**
     * @brief Get the next path object to explore
     *
     * @return Path
     */
    Path* fetch_branch(size_t id) {
        Path* path = nullptr;
        queues[id % N].try_pop(path);

        return path;
    }

    /**
     * @brief Add a path to the queue
     *
     * @param path Path to adds
     */
    void publish_branch(Path* path, size_t id) {
        queues[id % N].push(path);
    }

    /**
    * @brief Get the weight of the shortest path
    *
    * @return dist_t
    */
    dist_t shortest_distance() {
        Path* path = shortest_path.load();
        if (path == nullptr)
            return UINT64_MAX;

        return path->distance();
    }

    /**
    * @brief Get the size of the graph
    *
    * @return size_t
    */
    size_t number_of_nodes() {
        return graph->size();
    }


private:
    Graph* graph;
    std::atomic<size_t> paths_left;
    std::atomic<Path*> shortest_path;
    std::array<moodycamel::ConcurrentQueue<Path*>, N> queues;
    const size_t TOTAL_PATHS;
};

#endif //_thread_params_hpp