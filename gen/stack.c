#include <stdlib.h>

enum ntype {
        ADD = 1,
        SUB = 2,
        MUL = 4,
        DIV = 8,
        SIN = 16,
        COS = 32,
        TAN = 64,
        E       = 128,
        PI      = 256,
        X       = 512,
        VAL = 1024
};

struct node {
        struct node *left, *right;
        double  val;
        enum ntype type;
};


struct stack {
        struct node **arr;
        size_t size, max_size;
};

struct node* create_node(double value, enum ntype type) {
        struct node *node = malloc(sizeof(struct node));
        node->val = value;
        node->type = type;

        return node;
}

void free_node(struct node *node) {
        free(node);
}


struct stack* create_stack() {
        return calloc(sizeof(struct stack), 1);
}

void free_stack(struct stack *stack) {
        for (int i = 0; i < stack->size; i++) {
                free_node(stack->arr[i]);
        }

        free(stack);
}

void push_stack(struct node *node, struct stack *stack) {
        if (!stack->arr) {
                if (!stack->max_size) stack->max_size = 16;
                stack->arr = malloc(sizeof(struct node) * stack->max_size);
                stack->size = 0;
        }

        stack->arr[stack->size++] = node;

        if (stack->size >= stack->max_size) {
                stack->max_size *= 2;
                stack->arr = realloc(stack->arr, stack->max_size);
        }
}

struct node* pop_stack(struct stack *stack) {
        if (!stack->arr) return NULL;
        if (!stack->size) return NULL;

        return stack->arr[--stack->size];
}
