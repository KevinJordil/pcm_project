#include <cmath>

#include "threads_params.hpp"
#include "settings.hpp"
#include "tools.hpp"

ThreadParams::ThreadParams(Graph* graph) :
    graph(graph),
    paths_left(factorial[graph->size() - 1]),
    shortest_path(new Path()),
    queue(),
    TOTAL_PATHS(factorial[graph->size() - 1]) {
    if (graph->size() >= TSP_MAX_NODES)
        throw "Graphs must contain less than 20 edges";
}

ThreadParams::~ThreadParams() {
    delete shortest_path.load();
    Path* path = nullptr;
    while (previous_shortest_paths.try_pop(path))
        delete path;
}

uint64_t ThreadParams::get_paths_left() {
    return paths_left;
}

Path* ThreadParams::get_shortest_path() {
    return shortest_path;
}

void ThreadParams::set_shortest_path(Path* path) {
    Path* expected;
    Path* desired;

    do {
        expected = shortest_path.load();
        desired = path;

        // Stop trying to update if a better path appeared
        if (expected->distance() < desired->distance()) return;
    } while (!shortest_path.compare_exchange_weak(expected, desired));

    previous_shortest_paths.push(expected);
}

void ThreadParams::decrement_paths_left(size_t paths) {
    size_t expected;
    size_t desired;

    do {
        expected = paths_left.load();
        desired = expected - paths;
    } while (!paths_left.compare_exchange_weak(expected, desired));
}

Path* ThreadParams::fetch_branch() {
    Path* path = nullptr;
    queue.try_pop(path);

    return path;
}

void ThreadParams::publish_branch(Path* path) {
    queue.push(path);
}

dist_t ThreadParams::shortest_distance() {
    Path* path = shortest_path.load();
    if (path == nullptr)
        return UINT64_MAX;

    return path->distance();
}

size_t ThreadParams::number_of_nodes() {
    return graph->size();
}