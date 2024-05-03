#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "stack.h"

struct stringHeap *text_heap;
struct heap *data_heap;


void push_command(const char str[], struct stack *stack) {

        double d;
        char *end;
        
        struct node *node;

        if ((d = strtod(str, &end)) || (str != end)) {
                // Handle fpu number pushing
                node = create_node(d, VAL);
        } else if (strcmp(str, "sin") == 0) {
                // Handle fpu sin function
                node = create_node(0.0, SIN);
        } else if (strcmp(str, "cos") == 0) {
                // Handle fpu cos function
                node = create_node(0.0, COS);
        } else if (strcmp(str, "tan") == 0) {
                // Handle fpu tan function
                node = create_node(0.0, TAN);
        } else if (strcmp(str, "pi") == 0) {
                // Handle pi loading
                node = create_node(0.0, PI);
        } else {
                // Handle commands, e and x
                switch (str[0]) {
                        case '+':
                                node = create_node(0.0, ADD);
                                break;
                        case '-':
                                node = create_node(0.0, SUB);
                                break;
                        case '*':
                                node = create_node(0.0, MUL);
                                break;
                        case '/':
                                node = create_node(0.0, DIV);
                                break;
                        case 'e':
                                node = create_node(0.0, E);
                                break;
                        case 'x':
                                node = create_node(0.0, X);
                                break;
                        default:
                                printf("[-]Something went wrong while making tree from formula line");
                                return;
                }
        }

        if (node->type & (ADD | SUB | MUL | DIV)) {
                node->right = pop_stack(stack);
                node->left      = pop_stack(stack);
        } else if (node->type & (SIN | COS | TAN)) {
                node->left      = pop_stack(stack); 
        }

        push_stack(node, stack);
}

void tree_to_listing(struct node *node) {
        if (!node) return;
        
        if (node->type & (ADD | SUB | MUL | DIV)) {
                tree_to_listing(node->left);    
                tree_to_listing(node->right);   
        } else if (node->type & (TAN | SIN | COS)) {
                tree_to_listing(node->left);
        }
        int j;
        char *str;
        switch (node->type) {
                case VAL:
                        if ((j = search_heap(node->val, data_heap)) == -1) {
                                push_heap(node->val, data_heap);
                        }
                        j = (j == -1) ? data_heap->size : j;
                        asprintf(&str, "\n\tfld\tqword[const%d]\n", j);
                        push_stringHeap(str, text_heap);                        
                        break;
                
                case X:
                        asprintf(&str, "\n\tfld\tqword[ebp + 8]\n");
                        push_stringHeap(str, text_heap);
                        break;
                case PI:
                        asprintf(&str, "\n\tfldpi\n");
                        push_stringHeap(str, text_heap);
                        break;
                case E:
                        asprintf(&str, "\nfldl2e\n");
                        push_stringHeap(str, text_heap);
                        asprintf(&str, "fld1\n");
                        push_stringHeap(str, text_heap);
                        asprintf(&str, "fld\tst1\n");
                        push_stringHeap(str, text_heap);
                        asprintf(&str, "fprem\n");
                        push_stringHeap(str, text_heap);
                        asprintf(&str, "f2xm1\n");
                        push_stringHeap(str, text_heap);
                        asprintf(&str, "faddp\n");
                        push_stringHeap(str, text_heap);
                        asprintf(&str, "fscale\n");
                        push_stringHeap(str, text_heap);
                        asprintf(&str, "fstp\tst1\n");
                        push_stringHeap(str, text_heap);
                        break;
                case ADD:
                        asprintf(&str, "\n\tfaddp\n");
                        push_stringHeap(str, text_heap);
                        break;  
                case SUB:
                        asprintf(&str, "\n\tfsubp\n");
                        push_stringHeap(str, text_heap);
                        break;  
                case MUL:
                        asprintf(&str, "\n\tfmulp\n");
                        push_stringHeap(str, text_heap);
                        break;  
                case DIV:
                        asprintf(&str, "\n\tfdivp");
                        push_stringHeap(str, text_heap);
                        break;  
                case SIN:
                        asprintf(&str, "\n\tfsin");
                        push_stringHeap(str, text_heap);
                        break;
                case COS:
                        asprintf(&str, "\n\tfcos");
                        push_stringHeap(str, text_heap);
                        break;
                case TAN:
                        asprintf(&str, "\n\tfptan");
                        push_stringHeap(str, text_heap);
                        asprintf(&str, "\n\tfstp\tst0");
                        push_stringHeap(str, text_heap);
                        break;
                default:
                        printf("[-]Something went wrong while making listing from tree");
                        break;
        }
}

void print_text_section(FILE *file) {
        char *str;
        while (str = pop_stringHeap(text_heap)) {
                fprintf(file, "%s", str);
        } 
}

void print_data_section(FILE *file) {
        double val;
        int i = 0;
        while (data_heap->size) {
                val = pop_heap(data_heap);
                fprintf(file, "const%d:\tdq\t%lf\n", i, val);
                i++;
        }       
}

void create_listing(char *formula, size_t length, int index, FILE *output) {    

        char *end, *str = calloc(20, 1);
        
        int byte_offset = 0, byte_read = 0;
        
        struct stringHeap data_section = {NULL, 0, 0}, text_section = {NULL, 0, 0};
        
        struct stack *stack = create_stack();

        printf("%lu\n\n", length);

        while (
                        *(formula + byte_offset) != '\n' &&
                        *(formula + byte_offset) &&
                        sscanf(formula + byte_offset, "%s%n", str, &byte_read)) {
                
                push_command(str, stack);

                byte_offset += byte_read;
        }


        struct node *root = pop_stack(stack);
        
        char* temp;
        asprintf(&temp, "f%d:\n\tpush\tebp\n\tmov\tebp, esp", index);
        push_stringHeap(temp, text_heap);

        tree_to_listing(root);

        asprintf(&temp, "\tleave\n\tret\n\n");
        push_stringHeap(temp, text_heap);

        free_stack(stack);
        free(str);
}

int main(void) {
        
        char *spec_file = getenv("SPEC_FILE");

        if (!spec_file) {
                printf("SPEC_FILE not assigned\n");
                return 0;
        }

        FILE *input  = fopen(spec_file, "r");
        FILE *output = fopen("functions.asm", "w");

        double a, b;
        
        fscanf(input, "%lf %lf\n", &a, &b);

        printf("%lf %lf\n", a, b);      
        
        text_heap = create_stringHeap();
        data_heap = create_heap();

        for (int i = 0; i < 3; i++) {
                size_t n = 0;
                char *formula;
                int c = getdelim(&formula, &n, '\n', input);
                formula[c - 1] = 0; 
                
                create_listing(formula, n, 1, f1);
                free(formula);
        }

        fprintf(output, "section .data\n");
        print_data_section(output);

        fprintf(output, "\nsection .text\n");
        fprintf(output, "global f1\n");
        fprintf(output, "global f2\n");
        fprintf(output, "global f3\n");
        print_text_section(output);

        fclose(input);
        
        return 0;
}
