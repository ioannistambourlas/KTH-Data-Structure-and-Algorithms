#include "bst.h"
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

long long nano_seconds(LARGE_INTEGER *start, LARGE_INTEGER *stop)
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (stop->QuadPart - start->QuadPart) * 1000000000 / frequency.QuadPart;
}

tree *construct_tree()
{
    tree *t = (tree *)malloc(sizeof(tree));
    t->root = NULL;
    return t;
}

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

node *node_add(node *nd, int key)
{
    if (nd == NULL)
        return construct_node(key);
    if (nd->value == key)
        return nd;
    if (key < nd->value)
        nd->left = node_add(nd->left, key);
    else
        nd->right = node_add(nd->right, key);
    return nd;
}

void add_recursive(tree *tr, int value)
{
    tr->root = node_add(tr->root, value);
}

bool lookup(tree *tr, int value)
{
    node *cur = tr->root;
    while (cur != NULL)
    {
        if (cur->value == value)
            return true;
        if (value < cur->value)
            cur = cur->left;
        else
            cur = cur->right;
    }
    return false;
}

static void print_rec(node *nd)
{
    if (nd == NULL)
        return;
    print_rec(nd->left);
    printf("%d ", nd->value);
    print_rec(nd->right);
}

void print_tree(tree *tr)
{
    print_rec(tr->root);
    printf("\n");
}

stack *create_stack()
{
    stack *stk = (stack *)malloc(sizeof(stack));
    stk->top = NULL;
    return stk;
}

void push(stack *stk, node *nd)
{
    stack_node *sn = (stack_node *)malloc(sizeof(stack_node));
    sn->tn = nd;
    sn->next = stk->top;
    stk->top = sn;
}

node *pop(stack *stk)
{
    if (stk->top == NULL)
        return NULL;
    stack_node *sn = stk->top;
    node *nd = sn->tn;
    stk->top = sn->next;
    free(sn);
    return nd;
}

void free_stack(stack *stk)
{
    while (stk->top != NULL)
        pop(stk);
    free(stk);
}

void print_inorder(tree *tr)
{
    stack *stk = create_stack();
    node *cur = tr->root;

    while (cur != NULL || stk->top != NULL)
    {
        while (cur != NULL)
        {
            push(stk, cur);
            cur = cur->left;
        }
        cur = pop(stk);
        printf("%d ", cur->value);
        cur = cur->right;
    }
    printf("\n");
    free_stack(stk);
}
