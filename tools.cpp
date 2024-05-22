#include "tools.hpp"
#include "settings.hpp"

size_t factorial(size_t i) {
    static size_t cache[TSP_MAX_NODES] = {};

    if (!cache[i]) {
        size_t t = 1;
        for (size_t f = 2; f <= i; f++)
            t *= f;
        cache[i] = t;
    }

    return cache[i];
}
