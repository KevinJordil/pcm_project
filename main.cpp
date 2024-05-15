#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>
#include <numeric>
#include <chrono>
#include <stdio.h>
#include <inttypes.h>

#include "thread_worker.hpp"
#include "tspfile.hpp"
#include "concurrentqueue.hpp"

int main(int argc, char* argv[])
{
    char* fname = 0;
    int threads_number = 0;
    if (argc == 3)
    {
        fname = argv[1];
        threads_number = atoi(argv[2]);
    }
    else
    {
        fprintf(stderr, "usage: %s filename threads_number\n", argv[0]);
        exit(1);
    }

    Graph* graph = TSPFile::graph(fname);

    moodycamel::ConcurrentQueue<Path*> queue;
    std::vector<unsigned> cities_left(graph->size() - 1);
    std::iota(cities_left.begin(), cities_left.end(), 1);

    Path* path = new Path(graph);
    path->add(TSP_STARTING_CITY);
    queue.push(path);

    ThreadParams params(graph, &queue);
    std::vector<ThreadWorker> workers;
    std::vector<std::thread> threads;
    workers.reserve(threads_number);
    threads.reserve(threads_number);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < threads_number; i++) {
        workers.emplace_back(&params);
        threads.push_back(std::thread(&ThreadWorker::thread_work, &workers[i]));
    }

    for (int i = 0; i < threads_number; i++)
        threads[i].join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << params.get_shortest_path() << std::endl;

    std::cout << "Elapsed time: " << std::fixed << std::setprecision(2) << elapsed.count() << "s" << std::endl;

    // Free resources
    delete graph;

    return EXIT_SUCCESS;
}