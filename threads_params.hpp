#ifndef _thread_params_hpp
#define _thread_params_hpp

#include "path.hpp"
#include "graph.hpp"
#include "concurrentqueue.hpp"
//#include "queue.hpp"
#include "task.hpp"

#include <atomic>

class ThreadParams
{
public:
    ThreadParams(Graph* graph, moodycamel::ConcurrentQueue<Task*>* queue);

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
    const Path& get_shortest_path();

    /**
     * @brief Update the shortest path
     *
     * @param path Path to update
     */
    void update_shortest_path(Path* path);

    /**
     * @brief Decrement the paths left
     *
     * @param paths Number of paths to decrement
     */
    void decrement_paths_left(uint64_t paths);

    /**
     * @brief Get the next task object
     *
     * @return Task
     */
    Task* get_next_task();

    /**
     * @brief Add a task to the queue
     *
     * @param task Task to add
     */
    void add_task(Task* task);

    /**
    * @brief Get the weight of the shortest path
    *
    * @return uint64_t
    */
    uint64_t get_shortest_path_weight();

private:
    Graph* graph;
    std::atomic<uint64_t> paths_left;
    std::atomic<Path*> shortest_path;
    moodycamel::ConcurrentQueue<Task*>* queue;
    const uint64_t TOTAL_PATHS;
};

#endif //_thread_params_hpp