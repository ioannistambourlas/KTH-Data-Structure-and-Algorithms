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
        printf("Stack overflow!\n");
        return;
    }
    stk->array[stk->top++] = val;
}

int pop(stack *stk)
{
    if (stk->top == 0)
        return -1;
    return stk->array[--stk->top];
}

int main()
{
    stack *stk = new_stack(4);
    push(stk, 90);
    push(stk, 87);
    push(stk, 34);
    printf("pop: %d\n", pop(stk));
    printf("pop: %d\n", pop(stk));
    printf("pop: %d\n", pop(stk));
    free(stk->array);
    free(stk);
    return 0;
}
