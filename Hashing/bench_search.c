#include "hash.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    LARGE_INTEGER t_start, t_stop;

    codes *pnr = read_postcodes("postnummer.csv");
    if (pnr == NULL)
        return 1;

    int targets[] = {11115, 98499};
    int runs = 1000;

    printf("Benchmarking %d entries:\n", pnr->n);
    printf("%-10s | %-15s | %-15s\n", "Zip", "Linear (us)", "Binary (us)");
    printf("------------------------------------------------------\n");

    for (int i = 0; i < 2; i++)
    {
        int zip = targets[i];
        long long sum_linear = 0;
        long long sum_binary = 0;

        for (int k = 0; k < runs; k++)
        {
            QueryPerformanceCounter(&t_start);
            linear_int(pnr, zip);
            QueryPerformanceCounter(&t_stop);
            sum_linear += nano_seconds(&t_start, &t_stop);
        }

        for (int k = 0; k < runs; k++)
        {
            QueryPerformanceCounter(&t_start);
            binary_int(pnr, zip);
            QueryPerformanceCounter(&t_stop);
            sum_binary += nano_seconds(&t_start, &t_stop);
        }

        double avg_linear = (sum_linear / (double)runs) / 1000.0;
        double avg_binary = (sum_binary / (double)runs) / 1000.0;

        printf("%-10d | %-15.4f | %-15.4f\n", zip, avg_linear, avg_binary);
    }

    free(pnr->areas);
    free(pnr);
    return 0;
}
