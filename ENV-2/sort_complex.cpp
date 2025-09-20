#include "sort_complex.h"

void sortByMagnitude(std::vector<Complex>& arr) {
    const size_t n = arr.size();
    for (size_t i = 0; i < n; ++i) {
        size_t min_idx = i;
        double min_val = arr[i].abs();
        for (size_t j = i + 1; j < n; ++j) {
            double val = arr[j].abs();
            if (val < min_val) {
                min_val = val;
                min_idx = j;
            }
        }
        if (min_idx != i) {
            Complex tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
        }
    }
}
