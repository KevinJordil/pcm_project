#include "thread_worker.hpp"

#include <math.h>
#include <chrono>
#include <thread>
#include "tools.hpp"

using namespace std::literals::chrono_literals;

void sequential_bnb(Path* current, Path* shortest, ThreadParams* params);

void tsp_worker(ThreadParams* params) {
    while (true) {
        Path* branch = nullptr;

        do {
            branch = params->fetch_branch();

            // When everything is done, simply stop the thread
            if (params->get_paths_left() == 0) return;

            // Reduce the load on the queue when empty
            if (!branch) std::this_thread::sleep_for(100us);
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
                        params->publish_branch(new Path(*branch));
                    else
                        params->decrement_paths_left(factorial[branch->missing()]);

                    branch->pop();
                }
            }
        }

        delete branch;
    }
}

void sequential_bnb(Path* current, Path* shortest, ThreadParams* params) {
    // Path is finished, tiem to evaluate
    if (current->leaf()) {
        current->add(TSP_STARTING_CITY);
        if (current->distance() < shortest->distance()){
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