#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

long long nano_seconds(LARGE_INTEGER *t_start, LARGE_INTEGER *t_stop)
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (t_stop->QuadPart - t_start->QuadPart) * 1000000000 / frequency.QuadPart;
}

typedef struct node {
    int value;
    int size;
    struct node *left;
    struct node *right;
} node;

typedef struct heap {
    node *root;
} heap;

int get_size(node *nd)
{
    return nd ? nd->size : 0;
}

node *create_node(int val)
{
    node *nd = (node *)malloc(sizeof(node));
    nd->value = val;
    nd->size = 1;
    nd->left = NULL;
    nd->right = NULL;
    return nd;
}

node *node_add(node *nd, int val)
{
    if (nd == NULL)
        return create_node(val);

    if (val < nd->value)
    {
        int temp = nd->value;
        nd->value = val;
        val = temp;
    }

    if (get_size(nd->left) <= get_size(nd->right))
        nd->left = node_add(nd->left, val);
    else
        nd->right = node_add(nd->right, val);

    nd->size = 1 + get_size(nd->left) + get_size(nd->right);
    return nd;
}

void heap_add(heap *hp, int val)
{
    hp->root = node_add(hp->root, val);
}

node *remove_top(node *nd, int *min_val)
{
    if (nd == NULL)
        return NULL;

    if (nd->left == NULL && nd->right == NULL)
    {
        *min_val = nd->value;
        free(nd);
        return NULL;
    }

    node *smallest_child;
    if (nd->left && nd->right)
        smallest_child = (nd->left->value < nd->right->value) ? nd->left : nd->right;
    else
        smallest_child = nd->left ? nd->left : nd->right;

    nd->value = smallest_child->value;
    if (smallest_child == nd->left)
        nd->left = remove_top(nd->left, min_val);
    else
        nd->right = remove_top(nd->right, min_val);

    nd->size = 1 + get_size(nd->left) + get_size(nd->right);
    return nd;
}

int heap_remove(heap *hp)
{
    if (hp->root == NULL)
        return -1;
    int min;
    hp->root = remove_top(hp->root, &min);
    return min;
}

int heap_push(heap *hp, int incr)
{
    if (hp->root == NULL)
        return 0;

    node *cur = hp->root;
    cur->value += incr;
    int depth = 0;

    while (cur->left != NULL || cur->right != NULL)
    {
        node *smallest_child = NULL;

        if (cur->left && cur->right)
            smallest_child = (cur->left->value < cur->right->value) ? cur->left : cur->right;
        else
            smallest_child = cur->left ? cur->left : cur->right;

        if (cur->value > smallest_child->value)
        {
            int temp = cur->value;
            cur->value = smallest_child->value;
            smallest_child->value = temp;
            cur = smallest_child;
            cur->size = get_size(cur->left) + get_size(cur->right) + 1;
            depth++;
        }
        else
        {
            break;
        }
    }
    return depth;
}

int main()
{
    LARGE_INTEGER t_start, t_stop;
    srand((unsigned int)GetTickCount());

    heap *hp = (heap *)malloc(sizeof(heap));
    hp->root = NULL;

    int increments[] = {20, 40, 60, 80, 100};
    int n = 1023;

    for (int i = 0; i < n; i++)
        heap_add(hp, rand() % 10000 + 1);

    printf("%-5s | %-10s | %-6s | %-10s | %-10s | %-10s\n",
           "Incr", "Push(ns)", "Depth", "Rem(ns)", "Add(ns)", "Total(ns)");
    printf("----------------------------------------------------------------------\n");

    for (int i = 0; i < 5; i++)
    {
        QueryPerformanceCounter(&t_start);
        int d = heap_push(hp, increments[i]);
        QueryPerformanceCounter(&t_stop);
        long long push_ns = nano_seconds(&t_start, &t_stop);

        QueryPerformanceCounter(&t_start);
        int m = heap_remove(hp);
        QueryPerformanceCounter(&t_stop);
        long long rem_ns = nano_seconds(&t_start, &t_stop);

        QueryPerformanceCounter(&t_start);
        heap_add(hp, m + increments[i]);
        QueryPerformanceCounter(&t_stop);
        long long add_ns = nano_seconds(&t_start, &t_stop);

        printf("%-5d | %-10lld | %-6d | %-10lld | %-10lld | %-10lld\n",
               increments[i], push_ns, d, rem_ns, add_ns, rem_ns + add_ns);
    }

    return 0;
}
