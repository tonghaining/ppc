#include <algorithm>

typedef unsigned long long data_t;

void psort(int n, data_t *data) {
    // FIXME: Implement a more efficient parallel sorting algorithm for the CPU,
    // using the basic idea of quicksort.
    std::sort(data, data + n);
}
