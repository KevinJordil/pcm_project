#include "threads_params.hpp"
#include <cmath>

ThreadParams::ThreadParams(Graph* graph, Queue<Task*>* queue) : shortest_path(nullptr), TOTAL_PATHS(graph->size())
{
    this->graph = graph;
    this->queue = queue;
    if (graph->size() >= 20)
        throw "Graphs must contain less than 20 edges";

    //! Graph size != number of paths left!!
    uint64_t paths_left = tgamma(graph->size());
    this->paths_left = paths_left;
}

uint64_t ThreadParams::get_paths_left()
{
    return this->paths_left;
}

const Path& ThreadParams::get_shortest_path()
{
    return *this->shortest_path;
}

void ThreadParams::update_shortest_path(Path* path)
{
    Path* expected;
    Path* desired;

    do
    {
        //? Are we sure that the memory stays allocated for long enough?
        //? Smart pointers could help
        expected = this->shortest_path.load();
        desired = path;

        // Stop trying to update if a better path appeared
        if (expected != nullptr && expected->get_weight() < desired->get_weight())
            break;


    } while (!this->shortest_path.compare_exchange_weak(expected, desired));
}

void ThreadParams::decrement_paths_left(uint64_t paths)
{
    uint64_t expected;
    uint64_t desired;

    do
    {
        expected = this->paths_left.load();
        desired = expected - paths;
    } while (!this->paths_left.compare_exchange_weak(expected, desired));
}

Task* ThreadParams::get_next_task()
{
    return this->queue->pop();
}

void ThreadParams::add_task(Task* task)
{
    this->queue->push(task);
}

uint64_t ThreadParams::get_shortest_path_weight()
{
    Path* path = this->shortest_path.load();
    if (path == nullptr)
        return -1;

    return path->get_weight();
}