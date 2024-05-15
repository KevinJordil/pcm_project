#ifndef _thread_worker_hpp
#define _thread_worker_hpp

#include <vector>
#include <stdint.h>
#include "threads_params.hpp"

#define NUMBER_CITIES_ALONE 8

class ThreadWorker
{

public:
    ThreadWorker(ThreadParams* params);

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
    bool computation_done();


private:
    ThreadParams* params;
};

#endif // _thread_worker_hpp
