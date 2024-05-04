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
#include "queue.hpp"

#define NUMBER_THREADS 2
#define P10_UINT64 10000000000000000000ULL   /* 19 zeroes */
#define E10_UINT64 19
#define STRINGIZER(x)   # x
#define TO_STRING(x)    STRINGIZER(x)

static int print_u128_u(__uint128_t u128)
{
    int rc;
    if (u128 > UINT64_MAX)
    {
        __uint128_t leading = u128 / P10_UINT64;
        uint64_t  trailing = u128 % P10_UINT64;
        rc = print_u128_u(leading);
        rc += printf("%." TO_STRING(E10_UINT64) PRIu64, trailing);
    }
    else
    {
        uint64_t u64 = u128;
        rc = printf("%" PRIu64, u64);
    }
    return rc;
}

int main(int argc, char* argv[])
{
    char* fname = 0;
    if (argc == 2)
    {
        fname = argv[1];
    }
    else
    {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(1);
    }

    Graph* graph = TSPFile::graph(fname);

    Queue<Task*> queue;
    std::vector<unsigned> cities_left(graph->size() - 1);
    std::iota(cities_left.begin(), cities_left.end(), 1);

    Path path(graph);
    Task* first_task = new Task(path, cities_left);
    unsigned first_city = 0;
    first_task->add_city_to_path(first_city);
    queue.push(first_task);

    ThreadParams params(graph, &queue);
    std::vector<ThreadWorker> thread_workers;
    thread_workers.reserve(NUMBER_THREADS);
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    // Start the threads
    for (int i = 0; i < NUMBER_THREADS; i++)
    {
        thread_workers.push_back(ThreadWorker(params));
        threads.push_back(std::thread(&ThreadWorker::thread_work, &thread_workers[i], i));
    }

    // Wait for the threads to finish
    for (int i = 0; i < NUMBER_THREADS; i++)
    {
        threads[i].join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Print the shortest path
    Path shortest_path = params.get_shortest_path();
    std::cout << "shortest [";
    print_u128_u(shortest_path.get_weight());
    std::cout << "]: ";
    for (unsigned city : shortest_path.get_path())
    {
        std::cout << city << ", ";
    }
    std::cout << std::endl;

    std::cout << "Elapsed time: " << std::fixed << std::setprecision(2) << elapsed.count() << "s" << std::endl;

    // Free resources
    delete graph;

    return EXIT_SUCCESS;
}