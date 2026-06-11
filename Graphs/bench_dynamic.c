#include "graphs.h"
#include <limits.h>

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    if (argc < 3)
    {
        printf("usage: %s <from> <to>\n", argv[0]);
        exit(1);
    }

    map *trains = graph("trains.ansi.csv");
    if (trains == NULL)
    {
        printf("Error: could not load graph\n");
        return 1;
    }

    city *from = lookup(trains, argv[1]);
    city *to = lookup(trains, argv[2]);

    printf("Searching from [%s] to [%s]\n", from->name, to->name);

    city *path[60];
    LARGE_INTEGER t_start, t_stop;
    QueryPerformanceCounter(&t_start);
    int s = shortest_path_dynamic(from, to, path, 0, INT_MAX);
    QueryPerformanceCounter(&t_stop);

    long long wall = nano_seconds(&t_start, &t_stop);

    if (s > 0)
        printf("shortest path %d found in %.2f ms\n", s, (double)wall / 1000000);
    else
        printf("no path found\n");

    return 0;
}
