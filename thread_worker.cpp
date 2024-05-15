#include "thread_worker.hpp"

#include <math.h>
#include <chrono>
#include <thread>

using namespace std::literals::chrono_literals;


ThreadWorker::ThreadWorker(ThreadParams* params) : params(params) {}

void ThreadWorker::thread_work() {
    while (true) {
        Path* branch = nullptr;

        do {
            branch = params->fetch_branch();

            // When everything is done, simply stop the thread
            if (computation_done()) return;

            // Reduce the load on the queue when empty
            if (!branch) std::this_thread::sleep_for(100us);
        } while (!branch);

        // Branch is deep enough to traverse sequentially
        if (params->number_of_nodes() - branch->size() <= TSP_SEQUENTIAL_DEPTH) {
            // TODO: Setup environnement to call recursive function
            // TODO: Call recursive function
            // TODO: Increment the number of paths traversed
            // TODO: Is path better? If so, signal it
        }

        // Otherwise, create sub-branches for other threads to explore later
        else {
            for (node_t n = 0; n < params->number_of_nodes(); n++) {
                if (!branch->contains(n)) {
                    branch->add(n);

                    if (branch->distance() < params->shortest_distance())
                        params->publish_branch(new Path(*branch));
                    else {
                        // TODO: Increment the number of paths traversed by the number of pruned branches
                    }

                    branch->pop();
                }
            }
        }
    }
}

bool ThreadWorker::computation_done() {
    return params->get_paths_left() == 0;
}
