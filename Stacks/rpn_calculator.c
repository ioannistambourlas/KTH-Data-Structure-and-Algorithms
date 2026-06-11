#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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
    stack *stk = new_stack(10);
    int buf_size = 10;
    char *buffer = (char *)malloc(buf_size);
    bool run = true;

    printf("HP-35 pocket calculator\n");

    while (run)
    {
        printf(" > ");
        fgets(buffer, buf_size, stdin);

        if (strcmp(buffer, "\n") == 0)
        {
            run = false;
        }
        else if (strcmp(buffer, "+\n") == 0)
        {
            int a = pop(stk);
            int b = pop(stk);
            push(stk, a + b);
        }
        else if (strcmp(buffer, "-\n") == 0)
        {
            int a = pop(stk);
            int b = pop(stk);
            push(stk, b - a);
        }
        else if (strcmp(buffer, "*\n") == 0)
        {
            int a = pop(stk);
            int b = pop(stk);
            push(stk, a * b);
        }
        else if (strcmp(buffer, "/\n") == 0)
        {
            int a = pop(stk);
            int b = pop(stk);
            if (a == 0)
            {
                printf("Error: division by zero\n");
                return 0;
            }
            push(stk, b / a);
        }
        else
        {
            push(stk, atoi(buffer));
        }
    }

    printf("Result: %d\n", pop(stk));
    printf("I love reversed Polish notation, don't you?\n");
    return 0;
}
