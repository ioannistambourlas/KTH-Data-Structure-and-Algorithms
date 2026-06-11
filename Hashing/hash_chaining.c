#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

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

typedef struct cell {
    area data;
    struct cell *tail;
} cell;

typedef struct linked {
    cell *first;
} linked;

codes *read_postcodes(char *file)
{
    codes *postnr = (codes *)malloc(sizeof(codes));
    postnr->areas = (area *)malloc(20000 * sizeof(area));
    postnr->n = 0;

    FILE *fptr = fopen(file, "r");
    char lineptr[256];

    while (fgets(lineptr, 256, fptr) != NULL)
    {
        char *copy = strdup(lineptr);
        char *zip_str = strtok(copy, ",");
        char *name_str = strtok(NULL, ",");
        char *pop_str = strtok(NULL, ",");

        if (zip_str && name_str && pop_str)
        {
            int i = postnr->n;
            postnr->areas[i].zip = atoi(zip_str) * 100 + atoi(zip_str + 4);
            postnr->areas[i].name = strdup(name_str);
            postnr->areas[i].pop = atoi(pop_str);
            postnr->n++;
        }
        free(copy);
    }

    fclose(fptr);
    return postnr;
}

linked *linked_create()
{
    linked *lnk = (linked *)malloc(sizeof(linked));
    lnk->first = NULL;
    return lnk;
}

void linked_add(linked *lnk, area a)
{
    cell *new_c = (cell *)malloc(sizeof(cell));
    new_c->data = a;
    new_c->tail = lnk->first;
    lnk->first = new_c;
}

linked **build_hash_table(codes *postnr, int mod)
{
    linked **table = (linked **)malloc(mod * sizeof(linked *));
    for (int i = 0; i < mod; i++)
        table[i] = linked_create();

    for (int i = 0; i < postnr->n; i++)
    {
        int index = postnr->areas[i].zip % mod;
        linked_add(table[index], postnr->areas[i]);
    }
    return table;
}

bool lookup(linked **table, int mod, int zip)
{
    int index = zip % mod;
    cell *cur = table[index]->first;
    while (cur != NULL)
    {
        if (cur->data.zip == zip)
            return true;
        cur = cur->tail;
    }
    return false;
}

int main()
{
    codes *my_list = read_postcodes("postnummer.csv");
    if (my_list == NULL)
        return 1;

    int mod = 10000;
    linked **table = build_hash_table(my_list, mod);

    int iterations = 1000000;
    int zip_sthlm = 11115;
    int zip_pajala = 98499;

    LARGE_INTEGER start, stop;

    printf("Bucket hash table benchmark (%d iterations, mod %d)\n", iterations, mod);
    printf("----------------------------------------------------------\n");

    QueryPerformanceCounter(&start);
    for (int i = 0; i < iterations; i++)
        lookup(table, mod, zip_sthlm);
    QueryPerformanceCounter(&stop);
    long long total_sthlm = nano_seconds(&start, &stop);
    printf("Stockholm (111 15): %.2f ns avg\n", (double)total_sthlm / iterations);

    QueryPerformanceCounter(&start);
    for (int i = 0; i < iterations; i++)
        lookup(table, mod, zip_pajala);
    QueryPerformanceCounter(&stop);
    long long total_pajala = nano_seconds(&start, &stop);
    printf("Pajala    (984 99): %.2f ns avg\n", (double)total_pajala / iterations);

    return 0;
}
