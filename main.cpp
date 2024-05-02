#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <thread>
#include <numeric>

#include "thread_worker.hpp"
#include "tspfile.hpp"
#include "queue.hpp"

#define NUMBER_THREADS 2

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
    for (unsigned city : cities_left)
    {
        std::cout << city << " ";
    }

    Path path(graph);
    Task* first_task = new Task(path, cities_left);
    unsigned first_city = 0;
    first_task->add_city_to_path(first_city);
    queue.push(first_task);

    ThreadParams params(graph, &queue);
    std::thread threads[NUMBER_THREADS];

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
    std::cout << "shortest path cities: ";
    for (unsigned city : shortest_path.get_path())
    {
        std::cout << city << " ";
    }
    std::cout << std::endl; 

    // Free resources
    delete graph;

    return EXIT_SUCCESS;
}