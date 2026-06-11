#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

#define BUFFER 200

long long nano_seconds(LARGE_INTEGER *t_start, LARGE_INTEGER *t_stop)
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (t_stop->QuadPart - t_start->QuadPart) * 1000000000 / frequency.QuadPart;
}

typedef struct area {
    int zip;
    char *name;
    int pop;
} area;

typedef struct codes {
    area *areas;
    int n;
} codes;

codes *read_postcodes_direct(char *file)
{
    codes *postnr = (codes *)malloc(sizeof(codes));
    postnr->areas = (area *)calloc(100000, sizeof(area));

    FILE *fptr = fopen(file, "r");
    char lineptr[BUFFER];

    while (fgets(lineptr, BUFFER, fptr) != NULL)
    {
        char *copy = strdup(lineptr);
        char *zip_str = strtok(copy, ",");
        char *name_str = strtok(NULL, ",");
        char *pop_str = strtok(NULL, ",");

        if (zip_str && name_str)
        {
            int index = atoi(zip_str) * 100 + atoi(zip_str + 4);
            if (index >= 0 && index < 100000)
            {
                postnr->areas[index].zip = index;
                postnr->areas[index].name = strdup(name_str);
                postnr->areas[index].pop = pop_str ? atoi(pop_str) : 0;
            }
        }
        free(copy);
    }
    fclose(fptr);
    postnr->n = 100000;
    return postnr;
}

codes *read_postcodes_dense(char *file)
{
    codes *postnr = (codes *)malloc(sizeof(codes));
    postnr->areas = (area *)malloc(20000 * sizeof(area));

    FILE *fptr = fopen(file, "r");
    char lineptr[BUFFER];
    int k = 0;

    while (fgets(lineptr, BUFFER, fptr) != NULL)
    {
        char *copy = strdup(lineptr);
        char *zip_str = strtok(copy, ",");
        char *name_str = strtok(NULL, ",");
        char *pop_str = strtok(NULL, ",");

        if (zip_str && name_str)
        {
            postnr->areas[k].zip = atoi(zip_str) * 100 + atoi(zip_str + 4);
            postnr->areas[k].name = strdup(name_str);
            postnr->areas[k].pop = pop_str ? atoi(pop_str) : 0;
            k++;
        }
        free(copy);
    }
    fclose(fptr);
    postnr->n = k;
    return postnr;
}

char *lookup_direct(codes *pnr, int zip)
{
    if (zip < 0 || zip >= 100000 || pnr->areas[zip].name == NULL)
        return NULL;
    return pnr->areas[zip].name;
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

int main()
{
    LARGE_INTEGER t_start, t_stop;

    codes *pnr_direct = read_postcodes_direct("postnummer.csv");
    codes *pnr_dense = read_postcodes_dense("postnummer.csv");

    int targets[] = {11115, 98499};
    int runs = 10000;

    printf("%-10s | %-15s | %-15s\n", "Zip", "Binary (us)", "Direct (us)");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < 2; i++)
    {
        int target = targets[i];
        long long sum_binary = 0;
        long long sum_direct = 0;

        for (int k = 0; k < runs; k++)
        {
            QueryPerformanceCounter(&t_start);
            binary_int(pnr_dense, target);
            QueryPerformanceCounter(&t_stop);
            sum_binary += nano_seconds(&t_start, &t_stop);
        }

        for (int k = 0; k < runs; k++)
        {
            QueryPerformanceCounter(&t_start);
            lookup_direct(pnr_direct, target);
            QueryPerformanceCounter(&t_stop);
            sum_direct += nano_seconds(&t_start, &t_stop);
        }

        printf("%-10d | %-15.6f | %-15.6f\n",
               target,
               (sum_binary / (double)runs) / 1000.0,
               (sum_direct / (double)runs) / 1000.0);
    }

    return 0;
}
