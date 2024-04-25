#include <cstdlib>
#include <iomanip>
#include <iostream>

#include "thread_worker.hpp"
#include "graph.hpp"
#include "tspfile.hpp"

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

    Graph *g = TSPFile::graph(fname);

    uint64_t paths = 10;

    ThreadParams params(paths);

    return EXIT_SUCCESS;
}