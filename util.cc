#include <cstdint>
#include <cstdio>

#include "util.h"

void print_array(char const title[], uint32_t data[], long size) {
    puts(title);
    printf("{");
    for (long i = 0; i < size; ++i) {
        printf("%.3e, ", double(data[i]));
    }
    puts("}\n");
}

int div_ceil(int x, int y) {
    return x / y + (x % y > 0);
}