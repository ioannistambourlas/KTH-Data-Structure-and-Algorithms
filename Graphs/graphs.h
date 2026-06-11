#ifndef GRAPHS_H
#define GRAPHS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define MOD 103
#define BUFFER 256

typedef struct city city;
typedef struct connection connection;

typedef struct city {
    char *name;
    struct connection *connections;
    struct city *next;
} city;

typedef struct connection {
    struct city *destination;
    int distance;
    struct connection *next;
} connection;

typedef struct map {
    city **cities;
} map;

long long nano_seconds(LARGE_INTEGER *t_start, LARGE_INTEGER *t_stop);
int hash(char *name, int mod);
city *lookup(map *m, char *name);
void create_connection(city *src, city *dst, int dist);
map *graph(char *file);
void print_map(map *trains);

int shortest(city *from, city *to, int limit);
bool loop(city *dst, city *path[], int k);
int shortest_path(city *from, city *to, city *path[], int k);
int shortest_path_dynamic(city *from, city *to, city *path[], int k, int max);

#endif
