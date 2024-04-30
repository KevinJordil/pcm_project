#ifndef _thread_worker_hpp
#define _thread_worker_hpp

#include "task.hpp"

#include <vector>
#include <stdint.h>

#include "threads_params.hpp"

#define NUMBER_CITIES_ALONE 3

class ThreadWorker
{

public:
    ThreadWorker(ThreadParams &params);

    /**
     * @brief Thread main function
     *
     */
    void thread_work();

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
    Task *get_next_task();

    /**
     * @brief Add a task to the queue
     *
     * @param task Task to add
     */
    void add_task(Task *task);

private:
    ThreadParams &params;
    std::vector<Task*> local_tasks;
};

#endif // _thread_worker_hpp
