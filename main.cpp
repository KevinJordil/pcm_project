#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>
#include <numeric>
#include <chrono>
#include <cstdio>
#include <cinttypes>

#include "thread_worker.hpp"
#include "threads_params.hpp"
#include "tspfile.hpp"
#include "concurrentqueue.hpp"
#include "path.hpp"

int main(int argc, char* argv[])
{
    char* fname = 0;
    int num_threads = 0;

    if (argc == 3) {
        fname = argv[1];
        num_threads = atoi(argv[2]);
    }
    else {
        fprintf(stderr, "usage: %s filename num_threads\n", argv[0]);
        exit(1);
    }

    Graph* graph = TSPFile::graph(fname);
    global_graph = graph;
    Path* path = new Path();
    path->add(TSP_STARTING_CITY);

    ThreadParams<TSP_NUMBER_OF_QUEUES> params(graph);
    params.publish_branch(path, 0);
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < num_threads; i++)
        threads.emplace_back(tsp_worker<TSP_NUMBER_OF_QUEUES>, &params, i);

    for (int i = 0; i < num_threads; i++)
        threads[i].join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << params.get_shortest_path() << std::endl;

    std::cout << "Elapsed time: " << std::fixed << std::setprecision(2) << elapsed.count() << "s" << std::endl;

    // Free resources
    delete graph;

    return EXIT_SUCCESS;
}