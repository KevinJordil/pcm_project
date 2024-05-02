#include "thread_worker.hpp"

#include <math.h>

ThreadWorker::ThreadWorker(ThreadParams& params)
    : params(params)
{
}

void ThreadWorker::thread_work()
{
    // Print state
    std::cout << "thread_worker: Thread started" << std::endl;
    while (!should_stop())
    {
        std::cout << "thread_worker: while loop" << std::endl;
        Task* current_task = nullptr;
        unsigned fail_counter = 0;
        unsigned constexpr FAIL_THRESHOLD = 32;

        do
        {
            try {
                current_task = get_next_task();
            }
            catch (EmptyQueueException e) {
                fail_counter++;
            }
            // Display if current_task is null or not
            //? Wait ?
            //* Yes, that + batching tasks
        } while ((current_task == nullptr && !should_stop()) && fail_counter <= FAIL_THRESHOLD);

        if (should_stop() || fail_counter == FAIL_THRESHOLD)
        {
            std::cout << "thread_worker: Thread should stop" << std::endl;
            break;
        }

        std::cout << "thread_worker: thread have a new task" << std::endl;

        do
        {
            if (current_task->get_cities_left() > 1) {
                // For each city in cities left (ignore last city), create a new task and add it to the queue
                for (unsigned i = 0; i < current_task->get_cities_left() - 1; i++)
                {
                    // Copy pointer value current_task to new_task
                    Task* new_task = new Task(*current_task);
                    unsigned next_city = new_task->extract_city_to_visit(i);
                    __uint128_t weight = new_task->add_city_to_path(next_city);
                    __uint128_t shortest_weight = params.get_shortest_path_weight();

                    if(weight < shortest_weight)
                    {
                        if(new_task->get_cities_left() > NUMBER_CITIES_ALONE){
                            // Add the new task to the local tasks
                            std::cout << "thread_worker: Add new task to local tasks" << std::endl;
                            local_tasks.push_back(new_task);
                        }
                        else
                        {
                            // Add the new task to the queue
                            std::cout << "thread_worker: Add new task to queue" << std::endl;
                            add_task(new_task);
                        }
                    }
                    else
                    {
                        std::cout << "thread_worker: Cut cities number:" << tgamma(new_task->get_cities_left() + 1) << std::endl;
                        params.decrement_paths_left(tgamma(new_task->get_cities_left() + 1));
                        delete new_task;
                    }
                }
            }

            unsigned city = current_task->extract_city_to_visit(current_task->get_cities_left() - 1); 

            __uint128_t weight = current_task->add_city_to_path(city);
            __uint128_t shortest_weight = params.get_shortest_path_weight();

            // Print total paths left
            std::cout << "thread_worker: Paths left: " << params.get_paths_left() << std::endl;

            if (weight >= shortest_weight && current_task->get_cities_left() > 0)
            {
                // Remove the amount of paths that would be not calculated, factorial of the number of cities left
                std::cout << "thread_worker: Cut cities number:" << tgamma(current_task->get_cities_left() + 1) << std::endl;
                params.decrement_paths_left(tgamma(current_task->get_cities_left() + 1));
                break;
            }
        } while (current_task->get_cities_left() > 0);

        // Need to add this condition when we cut a branch and do a break
        if (current_task->get_cities_left() == 0)
        {
            // Calculate the return to the first city
            unsigned first_city = 0;
            __uint128_t weight = current_task->add_city_to_path(first_city);
            ;
            // The path is complete
            params.decrement_paths_left(1);

            // Check if the path is the shortest
            if (weight < params.get_shortest_path_weight())
            {
                std::cout << "thread_worker: Update shortest path" << std::endl;
                params.update_shortest_path(&current_task->get_current_path());
            }
            else {
                delete current_task;
            }
        }
        else {
            delete current_task;
        }
    }
}

bool ThreadWorker::should_stop()
{
    return params.get_paths_left() == 0;
}

Task* ThreadWorker::get_next_task()
{
    Task* task = nullptr;

    if (local_tasks.size() > 0)
    {
        //std::cout << "thread_worker: Get next task from local tasks" << std::endl;
        task = local_tasks.back();
        local_tasks.pop_back();
    }
    else
    {
        //std::cout << "thread_worker: Get next task from params" << std::endl;
        task = params.get_next_task();
    }

    return task;
}

void ThreadWorker::add_task(Task* task)
{
    params.add_task(task);
}