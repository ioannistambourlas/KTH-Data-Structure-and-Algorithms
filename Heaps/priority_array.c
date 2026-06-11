#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

long long nano_seconds(LARGE_INTEGER *t_start, LARGE_INTEGER *t_stop)
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (t_stop->QuadPart - t_start->QuadPart) * 1000000000 / frequency.QuadPart;
}

typedef struct heap {
    int *array;
    int size;
    int capacity;
} heap;

heap *create_heap(int cap)
{
    heap *hp = (heap *)malloc(sizeof(heap));
    hp->array = (int *)malloc(sizeof(int) * cap);
    hp->size = 0;
    hp->capacity = cap;
    return hp;
}

void heap_add(heap *hp, int value)
{
    if (hp->size == hp->capacity)
    {
        hp->capacity *= 2;
        hp->array = (int *)realloc(hp->array, hp->capacity * sizeof(int));
    }

    int cur = hp->size;
    hp->array[cur] = value;
    hp->size++;

    while (cur > 0)
    {
        int parent = (cur - 1) / 2;
        if (hp->array[cur] < hp->array[parent])
        {
            int tmp = hp->array[cur];
            hp->array[cur] = hp->array[parent];
            hp->array[parent] = tmp;
            cur = parent;
        }
        else
        {
            break;
        }
    }
}

int heap_remove(heap *hp)
{
    if (hp->size == 0)
        return -1;

    int min_val = hp->array[0];
    hp->array[0] = hp->array[hp->size - 1];
    hp->size--;

    int index = 0;
    while (1)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < hp->size && hp->array[left] < hp->array[smallest])
            smallest = left;
        if (right < hp->size && hp->array[right] < hp->array[smallest])
            smallest = right;

        if (smallest != index)
        {
            int tmp = hp->array[index];
            hp->array[index] = hp->array[smallest];
            hp->array[smallest] = tmp;
            index = smallest;
        }
        else
        {
            break;
        }
    }
    return min_val;
}

int heap_push(heap *hp, int incr)
{
    if (hp->size == 0)
        return 0;

    hp->array[0] += incr;
    int depth = 0;
    int index = 0;

    while (1)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < hp->size && hp->array[left] < hp->array[smallest])
            smallest = left;
        if (right < hp->size && hp->array[right] < hp->array[smallest])
            smallest = right;

        if (smallest != index)
        {
            int tmp = hp->array[index];
            hp->array[index] = hp->array[smallest];
            hp->array[smallest] = tmp;
            index = smallest;
            depth++;
        }
        else
        {
            break;
        }
    }
    return depth;
}

int compare_ll(const void *a, const void *b)
{
    long long x = *(long long *)a;
    long long y = *(long long *)b;
    return (x > y) - (x < y);
}

int main()
{
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000};
    int iters = 100;

    printf("%-10s | %-15s | %-15s\n", "Size (n)", "Add (ns)", "Rem (ns)");
    printf("----------------------------------------------------\n");

    for (int i = 0; i < 6; i++)
    {
        int n = sizes[i];
        long long *add_results = (long long *)malloc(sizeof(long long) * iters);
        long long *rem_results = (long long *)malloc(sizeof(long long) * iters);

        for (int t = 0; t < iters; t++)
        {
            heap *hp = create_heap(n + 10);
            LARGE_INTEGER t_start, t_stop;

            QueryPerformanceCounter(&t_start);
            for (int j = 0; j < n; j++)
                heap_add(hp, rand());
            QueryPerformanceCounter(&t_stop);
            add_results[t] = nano_seconds(&t_start, &t_stop) / n;

            QueryPerformanceCounter(&t_start);
            for (int j = 0; j < n; j++)
                heap_remove(hp);
            QueryPerformanceCounter(&t_stop);
            rem_results[t] = nano_seconds(&t_start, &t_stop) / n;

            free(hp->array);
            free(hp);
        }

        qsort(add_results, iters, sizeof(long long), compare_ll);
        qsort(rem_results, iters, sizeof(long long), compare_ll);

        printf("%-10d | %-15lld | %-15lld\n", n, add_results[iters / 2], rem_results[iters / 2]);

        free(add_results);
        free(rem_results);
    }

    return 0;
}
