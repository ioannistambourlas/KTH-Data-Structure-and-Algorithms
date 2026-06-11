#include "graphs.h"

long long nano_seconds(LARGE_INTEGER *t_start, LARGE_INTEGER *t_stop)
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (t_stop->QuadPart - t_start->QuadPart) * 1000000000 / frequency.QuadPart;
}

int hash(char *name, int mod)
{
    int h = 0;
    int i = 0;
    unsigned char c = 0;
    while ((c = name[i]) != 0)
    {
        h = (h * 31 + c) % mod;
        i++;
    }
    return h;
}

city *lookup(map *m, char *name)
{
    int h = hash(name, MOD);
    city *cur = m->cities[h];
    while (cur != NULL)
    {
        if (strcmp(name, cur->name) == 0)
            return cur;
        cur = cur->next;
    }

    city *new = (city *)malloc(sizeof(city));
    new->name = strdup(name);
    new->connections = NULL;
    new->next = m->cities[h];
    m->cities[h] = new;
    return new;
}

void create_connection(city *src, city *dst, int dist)
{
    connection *new = (connection *)malloc(sizeof(connection));
    new->destination = dst;
    new->distance = dist;
    new->next = src->connections;
    src->connections = new;
}

map *graph(char *file)
{
    city **cities = (city **)calloc(MOD, sizeof(city *));
    map *trains = (map *)malloc(sizeof(map));
    trains->cities = cities;

    FILE *fptr = fopen(file, "r");
    if (fptr == NULL)
        return NULL;

    char line[BUFFER];
    while (fgets(line, BUFFER, fptr))
    {
        char *copy = (char *)malloc(strlen(line) + 1);
        strcpy(copy, line);

        char *from_name = strtok(copy, ",");
        char *to_name = strtok(NULL, ",");
        unsigned int dist = atoi(strtok(NULL, ","));

        city *from = lookup(trains, from_name);
        city *to = lookup(trains, to_name);

        create_connection(from, to, dist);
        create_connection(to, from, dist);
        free(copy);
    }
    fclose(fptr);
    return trains;
}

void print_map(map *trains)
{
    for (int i = 0; i < MOD; i++)
    {
        city *cur = trains->cities[i];
        while (cur != NULL)
        {
            printf("City: %s\n", cur->name);
            connection *con = cur->connections;
            while (con != NULL)
            {
                printf("  -> %s (%d)\n", con->destination->name, con->distance);
                con = con->next;
            }
            cur = cur->next;
        }
    }
}

int shortest(city *from, city *to, int limit)
{
    if (from == to)
        return 0;

    int sofar = -1;
    connection *nxt = from->connections;
    while (nxt != NULL)
    {
        if (nxt->distance <= limit)
        {
            int d = shortest(nxt->destination, to, limit - nxt->distance);
            if (d >= 0 && (sofar == -1 || d + nxt->distance < sofar))
                sofar = d + nxt->distance;
        }
        nxt = nxt->next;
    }
    return sofar;
}

bool loop(city *dst, city *path[], int k)
{
    for (int i = 0; i < k; i++)
    {
        if (path[i] == dst)
            return true;
    }
    return false;
}

int shortest_path(city *from, city *to, city *path[], int k)
{
    if (from == to)
        return 0;

    path[k] = from;
    int sofar = -1;
    connection *nxt = from->connections;
    while (nxt != NULL)
    {
        if (!loop(nxt->destination, path, k))
        {
            int d = shortest_path(nxt->destination, to, path, k + 1);
            if (d >= 0 && (sofar == -1 || d + nxt->distance < sofar))
                sofar = d + nxt->distance;
        }
        nxt = nxt->next;
    }
    return sofar;
}

int shortest_path_dynamic(city *from, city *to, city *path[], int k, int max)
{
    if (from == to)
        return 0;

    path[k] = from;
    int best = -1;
    connection *nxt = from->connections;
    while (nxt != NULL)
    {
        if (!loop(nxt->destination, path, k) && nxt->distance <= max)
        {
            int d = shortest_path_dynamic(nxt->destination, to, path, k + 1, max - nxt->distance);
            if (d >= 0)
            {
                int total = d + nxt->distance;
                if (total < best || best == -1)
                {
                    best = total;
                    max = total;
                }
            }
        }
        nxt = nxt->next;
    }
    return best;
}
