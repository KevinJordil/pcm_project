#include "thread_worker.hpp"

#include <math.h>

ThreadWorker::ThreadWorker(ThreadParams &params)
    : params(params)
{
}

void ThreadWorker::thread_work(int id)
{
    while (!should_stop())
    {
        Task *current_task = nullptr;
        do
        {
            *current_task = get_next_task();
            // Wait ?
        } while (current_task == nullptr);

        do
        {
            unsigned city = current_task->extract_next_city_to_visit();
            unsigned weight = current_task->add_city_to_path(city);

            if (weight >= params.get_shortest_path().get_weight())
            {
                // Remove the amount of paths that would be not calculated, factorial of the number of cities left
                params.decrement_paths_left(tgamma(current_task->get_cities_left() + 1));
                break;
            }

            // If the task has more than NUMBER_CITIES_ALONE cities left, put the task back in the queue
            if (current_task->get_cities_left() > NUMBER_CITIES_ALONE)
            {
                Task new_task = *current_task;
                add_task(new_task);
            }
            else
            {
                Task new_task = *current_task;
                local_tasks.push_back(new_task);
            }

        } while (current_task->get_cities_left() > 0);

        // If the current task is complete
        if (current_task->get_cities_left() == 0)
        {
            // Calculate the return to the first city
            unsigned first_city = current_task->get_current_path().get_first_city();
            unsigned weight = current_task->add_city_to_path(first_city);

            // The path is complete
            params.decrement_paths_left(1);

            // Check if the path is the shortest
            if (weight < params.get_shortest_path().get_weight())
            {
                params.update_shortest_path(current_task->get_current_path());
            }
        }
    }
}

bool ThreadWorker::should_stop()
{
    return params.get_paths_left() == 0;
}

Task &ThreadWorker::get_next_task()
{
    Task *task = nullptr;
    if (local_tasks.size() > 0)
    {
        task = &local_tasks.back();
        local_tasks.pop_back();
    }
    else
    {
        // TODO get task from queue
    }
    return *task;
}

void ThreadWorker::add_task(Task &task)
{
    // TODO
}