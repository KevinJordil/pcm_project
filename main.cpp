#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>
#include <numeric>

#include "thread_worker.hpp"
#include "tspfile.hpp"
#include "queue.hpp"

#define NUMBER_THREADS 2

int main(int argc, char *argv[])
{
    char *fname = 0;
    if (argc == 2)
    {
        fname = argv[1];
    }
    else
    {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(1);
    }

    Graph *graph = TSPFile::graph(fname);

    Queue<Task> queue;
    std::vector<unsigned> cities_left(graph->size());
    std::iota(cities_left.begin(), cities_left.end(), 0);
    // Print state
    std::cout << "Cities left: ";
    for (unsigned city : cities_left)
    {
        std::cout << city << " ";
    }

    std::cout << "finish loop" << std::endl;

    Task *first_task = new Task(*graph, cities_left);

    std::cout << "Task created" << std::endl;

    queue.push(*first_task);

    std::cout << "First task created" << std::endl;

    ThreadParams params(*graph, &queue);

    std::cout << "Params created" << std::endl;

    std::thread threads[NUMBER_THREADS];

    std::cout << "Threads created" << std::endl;

    // Start the threads
    for (int i = 0; i < NUMBER_THREADS; i++)
    {
        ThreadWorker thread_worker(params);
        threads[i] = std::thread(&ThreadWorker::thread_work, &thread_worker);
    }

    // Wait for the threads to finish
    for (int i = 0; i < NUMBER_THREADS; i++)
    {
        threads[i].join();
    }

    // Print the shortest path
    Path shortest_path = params.get_shortest_path();
    std::cout << "Shortest path: " << shortest_path.get_weight() << std::endl;
    std::cout << "Cities: ";
    for (unsigned city : shortest_path.get_path())
    {
        std::cout << city << " ";
    }

    return EXIT_SUCCESS;
}