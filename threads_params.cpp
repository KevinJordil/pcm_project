#include "threads_params.hpp"

ThreadParams::ThreadParams(uint64_t paths_left) : shortest_path(nullptr)
{
    this->paths_left = &paths_left;
}

uint64_t ThreadParams::get_paths_left()
{
    return *this->paths_left;
}

Path ThreadParams::get_shortest_path()
{
    return *this->shortest_path;
}

void ThreadParams::update_shortest_path(Path &path)
{
    Path *expected;
    Path *desired;

    do
    {
        expected = this->shortest_path.load();
        desired = &path;
    } while (!this->shortest_path.compare_exchange_weak(expected, desired));
}

void ThreadParams::decrement_paths_left(uint64_t paths)
{
    uint64_t *expected;
    uint64_t *desired;

    do
    {
        expected = this->paths_left.load();
        desired = expected - paths;
    } while (!this->paths_left.compare_exchange_weak(expected, desired));
}