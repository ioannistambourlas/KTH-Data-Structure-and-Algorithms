#include <stdlib.h>
#include <stdio.h>

typedef struct stack
{
    int top;
    int size;
    int *array;
} stack;

stack *new_stack(int size)
{
    stack *stk = (stack *)malloc(sizeof(stack));
    stk->array = (int *)malloc(size * sizeof(int));
    stk->top = 0;
    stk->size = size;
    return stk;
}

void push(stack *stk, int val)
{
    if (stk->top == stk->size)
    {
        int new_size = stk->size * 2;
        int *copy = (int *)malloc(new_size * sizeof(int));
        for (int i = 0; i < stk->top; i++)
            copy[i] = stk->array[i];
        free(stk->array);
        stk->array = copy;
        stk->size = new_size;
    }
    stk->array[stk->top++] = val;
}

int pop(stack *stk)
{
    int min_size = 4;
    if (stk->top == 0)
        return -1;

    if (stk->top < stk->size / 4 && stk->size / 2 > min_size)
    {
        int new_size = stk->size / 2;
        int *copy = (int *)malloc(new_size * sizeof(int));
        for (int i = 0; i < stk->top; i++)
            copy[i] = stk->array[i];
        free(stk->array);
        stk->array = copy;
        stk->size = new_size;
    }

    return stk->array[--stk->top];
}

int main()
{
    stack *stk = new_stack(4);
    int n = 10;

    for (int i = 0; i < n; i++)
        push(stk, i + 30);

    int val;
    while ((val = pop(stk)) != -1)
        printf("pop: %d\n", val);

    free(stk->array);
    free(stk);
    return 0;
}
