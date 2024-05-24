
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

struct node* create_node(double value, enum ntype type);

void free_node(struct node *node);
	
struct stack* create_stack();

void free_stack(struct stack *stack);

void push_stack(struct node *node, struct stack *stack);

struct node* pop_stack(struct stack *stack);
