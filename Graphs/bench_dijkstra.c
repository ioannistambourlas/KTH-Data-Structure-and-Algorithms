#include "dijkstra.h"

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    if (argc < 3)
    {
        printf("usage: %s <from> <to>\n", argv[0]);
        exit(1);
    }

    map *trains = graph("europe.csv");
    if (trains == NULL)
    {
        printf("Error: could not load graph\n");
        return 1;
    }

    city *from = lookup(trains, argv[1]);
    city *to = lookup(trains, argv[2]);

    if (from == NULL || to == NULL)
    {
        printf("Error: city not found\n");
        return 1;
    }

    int total_cities = 10000;

    LARGE_INTEGER t_start, t_stop;
    QueryPerformanceCounter(&t_start);
    path *result = dijkstra(trains, from, to, total_cities);
    QueryPerformanceCounter(&t_stop);

    long long wall = nano_seconds(&t_start, &t_stop);

    if (result != NULL)
    {
        printf("From: %s  To: %s\n", from->name, to->name);
        printf("Shortest distance: %d min\n", result->dist);
        printf("Time taken: %.4f ms\n", (double)wall / 1000000);
    }
    else
    {
        printf("No path found.\n");
    }

    return 0;
}
