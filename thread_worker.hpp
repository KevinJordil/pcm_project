#ifndef _thread_worker_hpp
#define _thread_worker_hpp

#include <cstdint>
#include "threads_params.hpp"


#include <math.h>
#include <chrono>
#include <thread>
#include <cstdint>
#include "threads_params.hpp"
#include "tools.hpp"

using namespace std::literals::chrono_literals;

template <size_t N>
void sequential_bnb(Path* current, Path* shortest, ThreadParams<N>* params);

/**
 * @brief Thread main function
 *
 */
template <size_t N>
void tsp_worker(ThreadParams<N>* params, size_t id) {
    size_t fetch_id = 0;

    while (true) {
        Path* branch = nullptr;

        do {
            branch = params->fetch_branch(fetch_id);

            // When everything is done, simply stop the thread
            if (params->get_paths_left() == 0) return;

            // Try the next queue
            fetch_id++;
        } while (!branch);

        // Branch is deep enough to traverse sequentially
        if (branch->missing() <= TSP_SEQUENTIAL_DEPTH) {
            Path local_shortest(*params->get_shortest_path());
            sequential_bnb(branch, &local_shortest, params);

            if (local_shortest.distance() < params->shortest_distance())
                // TODO: Fix memory leak of old shortest 
                params->set_shortest_path(new Path(local_shortest));

            params->decrement_paths_left(factorial[branch->missing()]);
        }

        // Otherwise, create sub-branches for other threads to explore later
        else {
            for (node_t n = 1; n < branch->max(); n++) {
                if (!branch->contains(n)) {
                    branch->add(n);

                    if (branch->forecast_distance() < params->shortest_distance())
                        params->publish_branch(new Path(*branch), id);
                    else
                        params->decrement_paths_left(factorial[branch->missing()]);

                    branch->pop();
                }
            }
        }

        delete branch;
    }
}

template <size_t N>
void sequential_bnb(Path* current, Path* shortest, ThreadParams<N>* params) {
    // Path is finished, tiem to evaluate
    if (current->leaf()) {
        current->add(TSP_STARTING_CITY);
        if (current->distance() < shortest->distance()) {
            params->set_shortest_path(new Path(*current));
            *shortest = *params->get_shortest_path();
        }
        current->pop();
    }

    // Path still needs exploring, but seems promising
    else if (current->forecast_distance() < shortest->distance()) {
        for (node_t i = 1; i < current->max(); i++) {
            if (!current->contains(i)) {
                current->add(i);
                sequential_bnb(current, shortest, params);
                current->pop();
            }
        }
    }
}


#endif // _thread_worker_hpp
