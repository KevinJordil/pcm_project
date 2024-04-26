#include <cstdlib>
#include <iomanip>
#include <iostream>

#include "thread_worker.hpp"
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

    Graph *graph = TSPFile::graph(fname);

    ThreadParams params(*graph);

    return EXIT_SUCCESS;
}