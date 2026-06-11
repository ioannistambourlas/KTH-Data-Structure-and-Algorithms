#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct area {
    char *name;
    int zip;
    int pop;
} area;

typedef struct codes {
    area *areas;
    int n;
} codes;

codes *read_postcodes(char *file)
{
    codes *postnr = (codes *)malloc(sizeof(codes));
    postnr->areas = (area *)malloc(sizeof(area) * 15000);

    FILE *fptr = fopen(file, "r");
    if (!fptr)
        return NULL;

    char line[200];
    int i = 0;
    while (fgets(line, 200, fptr) != NULL)
    {
        char *copy = strdup(line);
        char *zip_str = strtok(copy, ",");
        int zip = atoi(zip_str) * 100 + atoi(zip_str + 3);

        postnr->areas[i].zip = zip;
        postnr->areas[i].name = strdup(strtok(NULL, ","));
        postnr->areas[i].pop = atoi(strtok(NULL, ","));
        i++;
        free(copy);
    }
    postnr->n = i;
    fclose(fptr);
    return postnr;
}

area **build_hash_table(codes *postnr, int mod)
{
    area **table = (area **)malloc(mod * sizeof(area *));
    for (int i = 0; i < mod; i++)
        table[i] = NULL;

    for (int i = 0; i < postnr->n; i++)
    {
        int index = postnr->areas[i].zip % mod;
        while (table[index] != NULL)
            index = (index + 1) % mod;
        table[index] = &postnr->areas[i];
    }
    return table;
}

int lookup(area **table, int mod, int zip)
{
    int comparisons = 0;
    int index = zip % mod;

    while (table[index] != NULL)
    {
        comparisons++;
        if (table[index]->zip == zip)
            return comparisons;
        index = (index + 1) % mod;
        if (comparisons >= mod)
            break;
    }
    return comparisons;
}

int main(void)
{
    int arraysizes[] = {9675, 12000, 12345, 13829, 18432, 20000, 30000};
    int elementtests[] = {11115, 98499, 16442, 11442, 11523, 11846, 18261, 41136, 23940, 41266, 21117, 30240};

    codes *data = read_postcodes("postnummer.csv");
    if (!data)
        return 1;

    for (int i = 0; i < 7; i++)
    {
        int mod = arraysizes[i];
        area **table = build_hash_table(data, mod);

        int total = 0;
        for (int j = 0; j < 12; j++)
            total += lookup(table, mod, elementtests[j]);

        printf("average comparisons for size %-5d is %.2f\n", mod, total / 12.0);
        free(table);
    }

    return 0;
}
