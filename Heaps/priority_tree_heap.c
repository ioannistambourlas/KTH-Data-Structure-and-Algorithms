#include <windows.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

long long nano_seconds(LARGE_INTEGER *start, LARGE_INTEGER *stop)
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (stop->QuadPart - start->QuadPart) * 1000000000 / frequency.QuadPart;
}

typedef struct node
{
    int value;
    int size;
    struct node *left;
    struct node *right;
} node;

typedef struct heap
{
    node *root;
} heap;

node *create_node(int value)
{
    node *nd = (node *)malloc(sizeof(node));
    nd->value = value;
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

node *node_add(node *nd, int value)
{
    if (nd == NULL)
        return create_node(value);

    if (value < nd->value)
    {
        int temp = nd->value;
        nd->value = value;
        value = temp;
    }

    if (node_size(nd->left) <= node_size(nd->right))
        nd->left = node_add(nd->left, value);
    else
        nd->right = node_add(nd->right, value);

    update_size(nd);
    return nd;
}

void heap_add(heap *hp, int value)
{
    hp->root = node_add(hp->root, value);
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

    if (nd->left->value <= nd->right->value)
    {
        nd->value = nd->left->value;
        nd->left = remove_top(nd->left);
    }
    else
    {
        nd->value = nd->right->value;
        nd->right = remove_top(nd->right);
    }

    update_size(nd);
    return nd;
}

int heap_remove(heap *hp)
{
    if (hp->root == NULL)
        return INT_MIN;

    int value = hp->root->value;
    hp->root = remove_top(hp->root);
    return value;
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

        if (cur->left != NULL && cur->right != NULL)
        {
            if (cur->left->value <= cur->right->value)
                smallest_child = cur->left;
            else
                smallest_child = cur->right;
        }
        else
        {
            smallest_child = (cur->left != NULL) ? cur->left : cur->right;
        }

        if (cur->value > smallest_child->value)
        {
            int temp = cur->value;
            cur->value = smallest_child->value;
            smallest_child->value = temp;
            cur = smallest_child;
            depth++;
        }
        else
        {
            break;
        }
    }

    return depth;
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

int main()
{
    heap *hp = (heap *)malloc(sizeof(heap));
    hp->root = NULL;
    int n = 1023;
    srand(42);

    for (int i = 0; i < n; i++)
        heap_add(hp, rand() % 10001);

    int increments[] = {100, 500, 1000, 2000, 5000, 10000};
    int num_tests = sizeof(increments) / sizeof(increments[0]);

    LARGE_INTEGER start, stop;

    printf("%-10s | %-10s | %-10s | %-10s | %-10s | %-10s\n",
           "Incr.", "Push (ns)", "Depth", "Rem (ns)", "Add (ns)", "Total (ns)");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < num_tests; i++)
    {
        int incr = increments[i];

        hp->root->value = rand() % 200;
        heap_push(hp, 0);

        QueryPerformanceCounter(&start);
        int d = heap_push(hp, incr);
        QueryPerformanceCounter(&stop);
        long long push_ns = nano_seconds(&start, &stop);

        QueryPerformanceCounter(&start);
        int val = heap_remove(hp);
        QueryPerformanceCounter(&stop);
        long long rem_ns = nano_seconds(&start, &stop);

        QueryPerformanceCounter(&start);
        heap_add(hp, val + incr);
        QueryPerformanceCounter(&stop);
        long long add_ns = nano_seconds(&start, &stop);

        printf("%-10d | %-10lld | %-10d | %-10lld | %-10lld | %-10lld\n",
               incr, push_ns, d, rem_ns, add_ns, rem_ns + add_ns);
    }

    free_heap(hp);
    return 0;
}
