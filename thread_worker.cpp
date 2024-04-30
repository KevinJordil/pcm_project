#include "thread_worker.hpp"

#include <math.h>

ThreadWorker::ThreadWorker(ThreadParams &params)
    : params(params)
{
}

void ThreadWorker::thread_work()
{
    // Print state
    std::cout << "Thread started" << std::endl;
    while (!should_stop())
    {
        std::cout << "while loop" << std::endl;
        Task *current_task = nullptr;
        do
        {
            current_task = get_next_task();
            // Display if current_task is null or not
            std::cout << "Current task!! " << std::endl;
            //? Wait ?
            //* Yes, that + batching tasks
        } while (current_task == nullptr && !should_stop());

        std::cout << "Get task!!!" << std::endl;

        if (should_stop())
        {
            break;
        }

        do
        {
            std::cout << "Do loop" << std::endl;
            //! We are silently ignoring unexplored paths!!
            //! The methods below modify the path and cities left in-place
            //! We are currently picking a single path and dropping all remaining others

            std::cout << "Get cities left" << current_task->get_cities_left() << std::endl;

            unsigned city = current_task->extract_next_city_to_visit();

            std::cout << "Extracted city: " << city << std::endl;

            // If the task has more than NUMBER_CITIES_ALONE cities left, put the task back in the queue
            if (current_task->get_cities_left() > NUMBER_CITIES_ALONE)
            {
                // Copy pointer value current_task to new_task
                Task *new_task = new Task();
                *new_task = *current_task;
                std::cout << "Adding task to queue" << std::endl;
                add_task(new_task);
                std::cout << "Task added to queue" << std::endl;
            }
            else if (current_task->get_cities_left() > 0)
            {
                // Using local_tasks is unnecessary overhead, we could run this iteratively
                Task *new_task = new Task();
                *new_task = *current_task;
                local_tasks.push_back(new_task);
            }

            unsigned weight = current_task->add_city_to_path(city);
            std::cout << "Added city to path" << std::endl;

            unsigned shortest_weight = params.get_shortest_path_weight();

            std::cout << "Get shortest path" << std::endl;

            if (weight >= shortest_weight)
            {
                // Remove the amount of paths that would be not calculated, factorial of the number of cities left
                std::cout << "Decrementing paths left" << std::endl;
                params.decrement_paths_left(tgamma(current_task->get_cities_left() + 1));
                break;
            }

            std::cout << "cities left: " << current_task->get_cities_left() << std::endl;

        } while (current_task->get_cities_left() > 0);

        // Need to add this condition when we cut a branch and do a break
        if (current_task->get_cities_left() == 0)
        {
            std::cout << "Cities left is 0" << std::endl;
            // Calculate the return to the first city
            unsigned first_city = 0;
            unsigned weight = current_task->add_city_to_path(first_city);

            std::cout << "Added first city to path" << std::endl;

            // The path is complete
            params.decrement_paths_left(1);

            std::cout << "Decrementing paths left" << std::endl;

            // Check if the path is the shortest
            if (weight < params.get_shortest_path_weight())
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

Task *ThreadWorker::get_next_task()
{
    Task *task = nullptr;
    if (local_tasks.size() > 0)
    {
        std::cout << "Get next task from local tasks" << std::endl;
        task = local_tasks.back();
        local_tasks.pop_back();
        std::cout << "Task extracted from local tasks" << std::endl;
    }
    else
    {
        std::cout << "Get next task from params" << std::endl;
        task = params.get_next_task();
    }
    return task;
}

void ThreadWorker::add_task(Task *task)
{
    params.add_task(task);
}