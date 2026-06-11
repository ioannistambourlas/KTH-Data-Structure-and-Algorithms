#include "hash.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AREAS 10000
#define BUFFER 100

long long nano_seconds(LARGE_INTEGER *start, LARGE_INTEGER *stop)
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (stop->QuadPart - start->QuadPart) * 1000000000 / frequency.QuadPart;
}

codes *read_postcodes(char *file)
{
    codes *postnr = (codes *)malloc(sizeof(codes));
    postnr->areas = (area *)malloc(sizeof(area) * AREAS);

    FILE *fptr = fopen(file, "r");
    char line[BUFFER];
    int k = 0;

    while (k < AREAS && fgets(line, BUFFER, fptr) != NULL)
    {
        char *copy = (char *)malloc(BUFFER);
        strcpy(copy, line);

        char *zip_str = strtok(copy, ",");
        int zip = atoi(zip_str) * 100 + atoi(zip_str + 3);

        postnr->areas[k].zip = zip;
        postnr->areas[k].name = strdup(strtok(NULL, ","));
        postnr->areas[k].pop = atoi(strtok(NULL, ","));
        k++;
        free(copy);
    }

    fclose(fptr);
    postnr->n = k;
    return postnr;
}

char *binary_int(codes *postnr, int zip)
{
    int left = 0;
    int right = postnr->n - 1;

    while (left <= right)
    {
        int mid = (left + right) / 2;
        if (postnr->areas[mid].zip == zip)
            return postnr->areas[mid].name;
        else if (postnr->areas[mid].zip < zip)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return NULL;
}

char *linear_int(codes *postnr, int zip)
{
    for (int i = 0; i < postnr->n; i++)
    {
        if (postnr->areas[i].zip == zip)
            return postnr->areas[i].name;
    }
    return NULL;
}
