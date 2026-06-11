#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

long long nano_seconds(LARGE_INTEGER *start, LARGE_INTEGER *stop)
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (stop->QuadPart - start->QuadPart) * 1000000000 / frequency.QuadPart;
}

typedef struct node {
    int value;
    struct node *left;
    struct node *right;
} node;

typedef struct tree {
    node *root;
} tree;

node *construct_node(int val)
{
    node *nd = (node *)malloc(sizeof(node));
    nd->value = val;
    nd->left = NULL;
    nd->right = NULL;
    return nd;
}

void free_node(node *nd)
{
    if (nd == NULL)
        return;
    free_node(nd->left);
    free_node(nd->right);
    free(nd);
}

void free_tree(tree *tr)
{
    free_node(tr->root);
    free(tr);
}

tree *construct_tree()
{
    tree *t = (tree *)malloc(sizeof(tree));
    t->root = NULL;
    return t;
}

void add_iterative(tree *tr, int value)
{
    if (tr->root == NULL)
    {
        tr->root = construct_node(value);
        return;
    }

    node *cur = tr->root;
    node *prv = NULL;

    while (cur != NULL)
    {
        if (cur->value == value)
            return;
        prv = cur;
        if (value < cur->value)
            cur = cur->left;
        else
            cur = cur->right;
    }

    node *nd = construct_node(value);
    if (value < prv->value)
        prv->left = nd;
    else
        prv->right = nd;
}

int tree_remove_min(tree *tr)
{
    if (tr->root == NULL)
        return -1;

    node *cur = tr->root;
    node *prv = NULL;

    while (cur->left != NULL)
    {
        prv = cur;
        cur = cur->left;
    }

    int result = cur->value;

    if (prv == NULL)
        tr->root = cur->right;
    else
        prv->left = cur->right;

    free(cur);
    return result;
}

int main()
{
    tree *tr = construct_tree();
    LARGE_INTEGER start, stop;

    QueryPerformanceCounter(&start);
    add_iterative(tr, rand() % 10000);
    QueryPerformanceCounter(&stop);
    double first_add = (double)nano_seconds(&start, &stop) / 1000.0;

    QueryPerformanceCounter(&start);
    int first_val = tree_remove_min(tr);
    QueryPerformanceCounter(&stop);
    double first_rem = (double)nano_seconds(&start, &stop) / 1000.0;

    for (int i = 0; i < 99; i++)
        add_iterative(tr, rand() % 10000);

    QueryPerformanceCounter(&start);
    add_iterative(tr, rand() % 10000);
    QueryPerformanceCounter(&stop);
    double hundredth_add = (double)nano_seconds(&start, &stop) / 1000.0;

    QueryPerformanceCounter(&start);
    tree_remove_min(tr);
    QueryPerformanceCounter(&stop);
    double hundredth_rem = (double)nano_seconds(&start, &stop) / 1000.0;

    printf("--- Individual Operation Timing ---\n");
    printf("1st   operation:  Add: %.3f us | Rem: %.3f us\n", first_add, first_rem);
    printf("100th operation:  Add: %.3f us | Rem: %.3f us\n", hundredth_add, hundredth_rem);

    free_tree(tr);
    return 0;
}
