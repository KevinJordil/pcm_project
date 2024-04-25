#ifndef _thread_worker_hpp
#define _thread_worker_hpp

#include "task.hpp"

#include <vector>
#include <stdint.h>

#include "threads_params.hpp"

class ThreadWorker
{

public:
    ThreadWorker(ThreadParams &params);

    /**
     * @brief Thread main function
     *
     * @param id Id of the thread
     */
    void thread_work(int id);

    /**
     * @brief Indicates if the threads should stop
     *
     * @return true
     * @return false
     */
    bool should_stop();

    /**
     * @brief Get the next task object
     *
     * @return Task&
     */
    Task &get_next_task();

    /**
     * @brief Add a task to the queue
     *
     * @param task Task to add
     */
    void add_task(Task &task);

    /**
     * @brief Update the global shortest path
     *
     * @param path Path to update
     * @param weight Weight of the path
     */
    void update_shortest_path(std::vector<unsigned> &path, unsigned weight);

private:
    Task task;
    ThreadParams &params;
};

#endif // _thread_worker_hpp
