#ifndef HASH_H
#define HASH_H

#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>

long long nano_seconds(LARGE_INTEGER *start, LARGE_INTEGER *stop);

typedef struct area {
    int zip;
    char *name;
    int pop;
} area;

typedef struct codes {
    area *areas;
    int n;
} codes;

codes *read_postcodes(char *file);
char *binary_int(codes *postnr, int zip);
char *linear_int(codes *postnr, int zip);

#endif
