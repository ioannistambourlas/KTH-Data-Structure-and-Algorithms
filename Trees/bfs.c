#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

long long nano_seconds(LARGE_INTEGER *start, LARGE_INTEGER *stop)
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (stop->QuadPart - start->QuadPart) * 1000000000 / frequency.QuadPart;
}

typedef struct tree_node
{
    int value;
    struct tree_node *left;
    struct tree_node *right;
} tree_node;

typedef struct tree
{
    tree_node *root;
} tree;

typedef struct q_element
{
    tree_node *tr_node;
    struct q_element *next;
} q_element;

typedef struct queue
{
    q_element *first;
    q_element *last;
} queue;

tree *construct_tree()
{
    tree *t = (tree *)malloc(sizeof(tree));
    t->root = NULL;
    return t;
}

tree_node *construct_node(int val)
{
    tree_node *nd = (tree_node *)malloc(sizeof(tree_node));
    nd->value = val;
    nd->left = NULL;
    nd->right = NULL;
    return nd;
}

void free_node(tree_node *nd)
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

    tree_node *cur = tr->root;
    tree_node *prv = NULL;

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

    tree_node *nd = construct_node(value);
    if (value < prv->value)
        prv->left = nd;
    else
        prv->right = nd;
}

queue *create_queue()
{
    queue *q = (queue *)malloc(sizeof(queue));
    q->first = NULL;
    q->last = NULL;
    return q;
}

bool empty(queue *q)
{
    return q->first == NULL;
}

void enqueue(queue *q, tree_node *nd)
{
    if (nd == NULL)
        return;
    q_element *el = (q_element *)malloc(sizeof(q_element));
    el->tr_node = nd;
    el->next = NULL;
    if (empty(q))
    {
        q->first = el;
        q->last = el;
    }
    else
    {
        q->last->next = el;
        q->last = el;
    }
}

tree_node *dequeue(queue *q)
{
    if (empty(q))
        return NULL;
    q_element *tmp = q->first;
    tree_node *nd = tmp->tr_node;
    q->first = tmp->next;
    if (q->first == NULL)
        q->last = NULL;
    free(tmp);
    return nd;
}

void bfs(tree *tr)
{
    if (tr->root == NULL)
        return;

    queue *q = create_queue();
    enqueue(q, tr->root);

    printf("BFS: ");
    while (!empty(q))
    {
        tree_node *cur = dequeue(q);
        printf("%d ", cur->value);
        enqueue(q, cur->left);
        enqueue(q, cur->right);
    }
    printf("\n");
    free(q);
}

typedef struct sequence
{
    queue *qu;
} sequence;

sequence *create_sequence(tree *tr)
{
    sequence *seq = (sequence *)malloc(sizeof(sequence));
    seq->qu = create_queue();
    if (tr->root != NULL)
        enqueue(seq->qu, tr->root);
    return seq;
}

int next_value(sequence *seq)
{
    if (empty(seq->qu))
        return -1;
    tree_node *cur = dequeue(seq->qu);
    enqueue(seq->qu, cur->left);
    enqueue(seq->qu, cur->right);
    return cur->value;
}

void free_sequence(sequence *seq)
{
    while (!empty(seq->qu))
        dequeue(seq->qu);
    free(seq->qu);
    free(seq);
}

int main()
{
    tree *tr = construct_tree();

    add_iterative(tr, 10);
    add_iterative(tr, 5);
    add_iterative(tr, 15);
    add_iterative(tr, 2);
    add_iterative(tr, 7);

    bfs(tr);

    sequence *seq = create_sequence(tr);
    printf("Step 1: %d\n", next_value(seq));
    printf("Step 2: %d\n", next_value(seq));

    add_iterative(tr, 20);

    int val;
    while ((val = next_value(seq)) != -1)
        printf("Next: %d\n", val);

    free_sequence(seq);
    free_tree(tr);
    return 0;
}
