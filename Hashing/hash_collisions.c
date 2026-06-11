#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

#define BUFFER 200

typedef struct area {
    int zip;
    char *name;
    int pop;
} area;

typedef struct codes {
    area *areas;
    int n;
} codes;

codes *read_postcodes(char *file)
{
    codes *postnr = (codes *)malloc(sizeof(codes));
    postnr->areas = (area *)malloc(100000 * sizeof(area));

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
            int index = atoi(zip_str) * 100 + atoi(zip_str + 3);
            if (index >= 0 && index < 100000)
            {
                postnr->areas[index].zip = index;
                postnr->areas[index].name = name_str;
                postnr->areas[index].pop = pop_str ? atoi(pop_str) : 0;
            }
        }
    }
    fclose(fptr);
    postnr->n = 100000;
    return postnr;
}

void collisions(codes *postnr, int mod)
{
    int mx = 20;
    int *data = (int *)calloc(mod, sizeof(int));
    int *cols = (int *)calloc(mx, sizeof(int));

    for (int i = 0; i < postnr->n; i++)
    {
        int index = postnr->areas[i].zip % mod;
        data[index]++;
    }

    int sum = 0;
    for (int i = 0; i < mod; i++)
    {
        sum += data[i];
        if (data[i] < mx)
            cols[data[i]]++;
    }

    printf("%d (%d) : ", mod, sum);
    for (int i = 1; i < mx; i++)
        printf("%6d ", cols[i]);
    printf("\n");

    free(data);
    free(cols);
}

int main()
{
    codes *pnr = read_postcodes("postnummer.csv");
    if (!pnr)
        return 1;

    printf("\n%-7s %-7s : %6s %6s %6s %6s %6s %6s %6s %6s %6s %6s\n",
           "Mod", "(Sum)", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10");
    printf("--------------------------------------------------------------------------------\n");

    collisions(pnr, 10000);
    collisions(pnr, 13513);
    collisions(pnr, 15000);
    collisions(pnr, 17389);
    collisions(pnr, 18500);
    collisions(pnr, 20000);

    printf("--------------------------------------------------------------------------------\n");
    return 0;
}
