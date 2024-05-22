#include <cmath>

#include "threads_params.hpp"
#include "settings.hpp"
#include "tools.hpp"

ThreadParams::ThreadParams(Graph* graph, moodycamel::ConcurrentQueue<Path*>* queue) : shortest_path(nullptr), TOTAL_PATHS(factorial(graph->size() - 1))
{
    this->graph = graph;
    this->queue = queue;
    if (graph->size() >= TSP_MAX_NODES)
        throw "Graphs must contain less than 20 edges";

    size_t paths_left = factorial(graph->size() - 1);
    this->paths_left = paths_left;
}

uint64_t ThreadParams::get_paths_left()
{
    return this->paths_left;
}

Path* ThreadParams::get_shortest_path()
{
    return this->shortest_path;
}

void ThreadParams::set_shortest_path(Path* path) {
    Path* expected;
    Path* desired;

    do {
        expected = this->shortest_path.load();
        desired = path;

        // Stop trying to update if a better path appeared
        if (expected != nullptr && expected->distance() < desired->distance()) break;
    } while (!this->shortest_path.compare_exchange_weak(expected, desired));
}

void ThreadParams::decrement_paths_left(size_t paths) {
    size_t expected;
    size_t desired;

    do {
        expected = this->paths_left.load();
        desired = expected - paths;
    } while (!this->paths_left.compare_exchange_weak(expected, desired));
}

Path* ThreadParams::fetch_branch() {
    Path* path = nullptr;
    this->queue->try_pop(path);

    return path;
}

void ThreadParams::publish_branch(Path* path) {
    this->queue->push(path);
}

dist_t ThreadParams::shortest_distance() {
    Path* path = this->shortest_path.load();
    if (path == nullptr)
        return UINT64_MAX;

    return path->distance();
}

size_t ThreadParams::number_of_nodes() {
    return graph->size();
}