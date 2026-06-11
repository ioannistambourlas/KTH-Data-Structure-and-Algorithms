#include "dijkstra.h"

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
    static int counter = 0;
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
    new->id = counter++;
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

node *create_node(path *p)
{
    node *nd = (node *)malloc(sizeof(node));
    nd->p = p;
    nd->size = 1;
    nd->left = NULL;
    nd->right = NULL;
    return nd;
}

int node_size(node *nd)
{
    if (nd == NULL)
        return 0;
    return nd->size;
}

void update_size(node *nd)
{
    if (nd != NULL)
        nd->size = 1 + node_size(nd->left) + node_size(nd->right);
}

node *node_add(node *nd, path *p)
{
    if (nd == NULL)
        return create_node(p);

    if (p->dist < nd->p->dist)
    {
        path *temp = nd->p;
        nd->p = p;
        p = temp;
    }

    if (node_size(nd->left) < node_size(nd->right))
        nd->left = node_add(nd->left, p);
    else
        nd->right = node_add(nd->right, p);

    update_size(nd);
    return nd;
}

void heap_add(heap *hp, path *p)
{
    hp->root = node_add(hp->root, p);
}

node *remove_top(node *nd)
{
    if (nd == NULL)
        return NULL;

    if (nd->left == NULL && nd->right == NULL)
    {
        free(nd);
        return NULL;
    }

    if (nd->left == NULL)
    {
        node *tmp = nd->right;
        free(nd);
        return tmp;
    }

    if (nd->right == NULL)
    {
        node *tmp = nd->left;
        free(nd);
        return tmp;
    }

    if (nd->left->p->dist <= nd->right->p->dist)
    {
        nd->p = nd->left->p;
        nd->left = remove_top(nd->left);
    }
    else
    {
        nd->p = nd->right->p;
        nd->right = remove_top(nd->right);
    }

    update_size(nd);
    return nd;
}

path *heap_remove(heap *hp)
{
    if (hp->root == NULL)
        return NULL;

    path *res = hp->root->p;
    hp->root = remove_top(hp->root);
    return res;
}

void free_node(node *nd)
{
    if (nd != NULL)
    {
        free_node(nd->left);
        free_node(nd->right);
        free(nd);
    }
}

void free_heap(heap *hp)
{
    free_node(hp->root);
    free(hp);
}

path *dijkstra(map *trains, city *from, city *to, int total_cities)
{
    heap *hp = (heap *)malloc(sizeof(heap));
    hp->root = NULL;

    path **done = (path **)calloc(total_cities, sizeof(path *));

    path *initial = (path *)malloc(sizeof(path));
    initial->cur = from;
    initial->prev = NULL;
    initial->dist = 0;
    heap_add(hp, initial);

    path *result = NULL;

    while (hp->root != NULL)
    {
        path *p = heap_remove(hp);
        city *c = p->cur;

        if (done[c->id] != NULL)
        {
            free(p);
            continue;
        }
        done[c->id] = p;

        if (c == to)
        {
            result = p;
            break;
        }

        connection *con = c->connections;
        while (con != NULL)
        {
            city *s = con->destination;
            if (done[s->id] == NULL)
            {
                path *new = (path *)malloc(sizeof(path));
                new->cur = s;
                new->prev = p;
                new->dist = p->dist + con->distance;
                heap_add(hp, new);
            }
            con = con->next;
        }
    }

    while (hp->root != NULL)
    {
        path *tmp = heap_remove(hp);
        free(tmp);
    }
    free(hp);
    free(done);
    return result;
}
