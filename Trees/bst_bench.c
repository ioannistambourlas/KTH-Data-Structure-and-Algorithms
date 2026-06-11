#include "bst.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main()
{
    LARGE_INTEGER t_start, t_stop;
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000};
    int runs = 1000;

    for (int i = 0; i < 6; i++)
    {
        int n = sizes[i];
        long long min = LLONG_MAX;

        for (int k = 0; k < runs; k++)
        {
            tree *tr = construct_tree();
            for (int j = 0; j < n; j++)
                add_recursive(tr, rand());

            QueryPerformanceCounter(&t_start);
            add_recursive(tr, rand());
            QueryPerformanceCounter(&t_stop);

            long long time = nano_seconds(&t_start, &t_stop);
            if (time < min)
                min = time;

            free_tree(tr);
        }
        printf("%d %.2f us\n", n, min / 1000.0);
    }

    return 0;
}
