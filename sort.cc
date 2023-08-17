#include "sort.h"

void array_swap(uint32_t array[], long a, long b) {
    uint32_t temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

long quicksort_partition(uint32_t array[], long low, long high) {
    uint32_t pivot = array[high]; //Perhaps make this median of low, mid, high
    long i = low-1;
    for (long j = low; j < high; ++j) {
        if (array[j] < pivot) {
            ++i;
            array_swap(array, i, j);
        }
    }

    ++i;
    array_swap(array, i, high);
    return i;
}

void quicksort(uint32_t array[], long low, long high) {
    long partition;
    //Nothing to do in this round
    if (low >= high || low < 0) {
        return;
    }

    partition = quicksort_partition(array, low, high);

    quicksort(array, low, partition-1);
    quicksort(array, partition+1, high);
}

void bubble_sort(uint32_t array[], long start, long end) {
    for (long i = start; i < end; ++i) {
        for (long j = start; j+1 < end; ++j) {
            if (array[j] > array[j+1]) {
                array_swap(array, j, j+1);
            }
        }
    }  
}

void stl_sort(uint32_t array[], long start, long end) {
    std::vector<uint32_t> v(array + start, array + end);
    std::sort(v.begin(), v.end());
    // Copy results back to array
    for (size_t i = 0; i < v.size(); i++) {
        array[start + i] = v[i];
    }
}

void sort(uint32_t array[], long size, SortType sortType) {
    switch (sortType) {
        case quick:
            //Needs some special treatment, as algorithm is end-inclusive
            quicksort(array, 0, size-1);
            break;
        case bubble:
            bubble_sort(array, 0, size);
            break;
        case stl:
            stl_sort(array, 0, size);
            break;
        default:
            quicksort(array, 0, size);
            break;
    }
}