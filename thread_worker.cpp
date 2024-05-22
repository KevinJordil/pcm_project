#include "thread_worker.hpp"

#include <math.h>
#include <chrono>
#include <thread>
#include "tools.hpp"

using namespace std::literals::chrono_literals;

void sequential_bnb(Path* current, Path* shortest, dist_t* shortest_distance);

void tsp_worker(ThreadParams* params) {
    while (true) {
        Path* branch = nullptr;
        unsigned deadlock_detector = 0;

        do {
            branch = params->fetch_branch();

            // When everything is done, simply stop the thread
            if (params->get_paths_left() == 0) return;

            // Reduce the load on the queue when empty
            if (!branch) std::this_thread::sleep_for(100us);

            if (deadlock_detector++ == 32)
                std::cout << "Deadlocked! paths left: " << params->get_paths_left() << std::endl;
        } while (!branch);

        // Branch is deep enough to traverse sequentially
        if (branch->max() - branch->size() <= TSP_SEQUENTIAL_DEPTH) {
            Path local_shortest(nullptr);
            dist_t local_shortest_distance = params->shortest_distance();
            sequential_bnb(branch, &local_shortest, &local_shortest_distance);

            if (local_shortest_distance < params->shortest_distance())
                // TODO: Fix memory leak of old shortest 
                params->set_shortest_path(new Path(local_shortest));
            
            params->decrement_paths_left(factorial(TSP_SEQUENTIAL_DEPTH));
        }

        // Otherwise, create sub-branches for other threads to explore later
        else {
            for (node_t n = 1; n < branch->max(); n++) {
                if (!branch->contains(n)) {
                    branch->add(n);

                    if (branch->distance() < params->shortest_distance())
                        params->publish_branch(new Path(*branch));
                    else
                        params->decrement_paths_left(factorial(branch->max() - branch->size()));

                    branch->pop();
                }
            }
        }

        delete branch;
    }
}

void sequential_bnb(Path* current, Path* shortest, dist_t* shortest_distance) {
	if (current->leaf()) {
		// this is a leaf
		current->add(0);
		if (current->distance() < *shortest_distance) {
			*shortest = *current;
            *shortest_distance = shortest->distance();
        }
		current->pop();
	} 
    else {
		if (current->distance() < *shortest_distance) {
			// continue branching
			for (node_t i = 1; i < current->max(); i++) {
				if (!current->contains(i)) {
					current->add(i);
					sequential_bnb(current, shortest, shortest_distance);
					current->pop();
				}
			}
		}
        else {
            // current already >= shortest known so far, bound
        }
	}
}