#include "thread_worker.hpp"

ThreadWorker::ThreadWorker(ThreadParams &params)
    : params(params)
{
}

void ThreadWorker::thread_work(int id)
{
    // TODO
}

bool ThreadWorker::should_stop()
{
    return params.get_paths_left() == 0;
}
