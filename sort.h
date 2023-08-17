#ifndef SORT_H
#define SORT_H

#include <chrono>
#include <algorithm>
#include <vector>

enum SortType {quick, bubble, stl};

void sort(uint32_t array[], long size, SortType sortType);

void array_swap(uint32_t array[], long a, long b);

long quicksort_partition(uint32_t array[], long low, long high);

void quicksort(uint32_t array[], long low, long high);

void bubble_sort(uint32_t array[], long start, long end);

void stl_sort(uint32_t array[], long start, long end);

#endif