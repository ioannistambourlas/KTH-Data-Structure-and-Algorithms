#ifndef TREES_H
#define TREES_H

#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>

long long nano_seconds(LARGE_INTEGER *start, LARGE_INTEGER *stop);

typedef struct node {
    int value;
    struct node *left;
    struct node *right;
} node;

typedef struct tree {
    node *root;
} tree;

typedef struct stack_node {
    node *tn;
    struct stack_node *next;
} stack_node;

typedef struct stack {
    stack_node *top;
} stack;

tree *construct_tree();
void free_tree(tree *tr);
node *construct_node(int val);
void free_node(node *nd);
node *node_add(node *nd, int key);

void add_iterative(tree *tr, int value);
void add_recursive(tree *tr, int value);
bool lookup(tree *tr, int value);

void print_tree(tree *tr);
void print_inorder(tree *tr);

stack *create_stack();
void push(stack *stk, node *nd);
node *pop(stack *stk);
void free_stack(stack *stk);

#endif
