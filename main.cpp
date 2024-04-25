#include <cstdlib>
#include <iomanip>
#include <iostream>

#include "threads_params.hpp"

int main(int argc, char *argv[])
{
    std::cout << "argc == " << argc << '\n';

    uint64_t paths = 10;

    ThreadParams params(paths);

    return EXIT_SUCCESS;
}