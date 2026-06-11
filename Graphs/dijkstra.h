#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define MOD 103
#define BUFFER 256

typedef struct city city;
typedef struct connection connection;

typedef struct path {
    city *cur;
    struct path *prev;
    int dist;
} path;

typedef struct city {
    char *name;
    struct connection *connections;
    int id;
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

typedef struct node {
    path *p;
    int size;
    struct node *left;
    struct node *right;
} node;

typedef struct heap {
    node *root;
} heap;

long long nano_seconds(LARGE_INTEGER *t_start, LARGE_INTEGER *t_stop);
int hash(char *name, int mod);
city *lookup(map *m, char *name);
void create_connection(city *src, city *dst, int dist);
map *graph(char *file);
path *dijkstra(map *trains, city *from, city *to, int total_cities);

node *create_node(path *p);
int node_size(node *nd);
void update_size(node *nd);
node *node_add(node *nd, path *p);
void heap_add(heap *hp, path *p);
node *remove_top(node *nd);
path *heap_remove(heap *hp);
void free_node(node *nd);
void free_heap(heap *hp);

#endif
