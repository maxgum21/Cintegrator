#define _GNU_SOURCE 	// need this to get asprintf function to manage heap strings

#ifndef KEY				// default key is "c" which stands for chord method
#define KEY = 'c'
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "stack.h"

struct sheap *text_heap; // string heap to keep track of section.text
struct dheap *data_heap; // double heap to keep track of in-file constants in section.data


void push_command(const char str[], struct stack *stack) {

		/*
			Builds a tree node by node
			by pushing nodes onto stack
		*/

        double d;
        char *end;
        
        struct node *node;

        if ((d = strtod(str, &end)) || (str != end)) {	// from strtod output there's no way of knowing if it failed or read a 0.0
                // Handle fpu number pushing			   that's why i use end to see if it read something or failed
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
                                //printf("[-] Something went wrong while making tree from formula line");
                                return;
                }
        }

        if (node->type & (ADD | SUB | MUL | DIV)) { 		// if the operation is binary,
                node->right = pop_stack(stack);				// then put node of first operand
                node->left      = pop_stack(stack);			// in the left child and second in the right
        } else if (node->type & (SIN | COS | TAN)) {		
                node->left      = pop_stack(stack); 		// if the operation is unary
        }													// then put the operand in the left child node

        push_stack(node, stack);
}

void tree_to_listing(struct node *node) {
        if (!node) return;

        char *str;

		/*
			This function recursively creates function code from the tree.
			The functions aim is to never load more than 2 items on the
			fpu stack at a time, thus every function takes all needed
			arguments and leaves only the result instead of them.
			This is done to prevent fpu stack overflow.
		*/
        
        if (node->type & (ADD | SUB | MUL | DIV)) {
				if (node->left->type & (VAL | PI | E | X)) {
					tree_to_listing(node->right);   // if in "A B OP" A is a constant,
					tree_to_listing(node->left);    // B is a RPN module,
					asprintf(&str, "\n\tfxch\n"); 	// then it gets B recursively first
                    push_sheap(str, text_heap);		// before loading A to prevent FPU stack overflow
				} else {
					tree_to_listing(node->left);    // in "A B OP" finds A, then B
					tree_to_listing(node->right);   // then does A OP B
				}					
        } else if (node->type & (TAN | SIN | COS)) {
                tree_to_listing(node->left);
        }
        int j;
        switch (node->type) {
                case VAL:
                        if ((j = search_dheap(node->val, data_heap)) == -1) { 	// check if this constant
                                push_dheap(node->val, data_heap);				// already exist and
                        }														// if it does use that constant
                        j = (j == -1) ? data_heap->size - 1 : j;
                        asprintf(&str, "\n\tfld\tqword[const%d]\n", j + 1);
                        push_sheap(str, text_heap);                        
                        break;
                
                case X:
                        asprintf(&str, "\n\tfld\tqword[ebp + 8]\n");
                        push_sheap(str, text_heap);
                        break;
                case PI:
                        asprintf(&str, "\n\tfldpi\n");
                        push_sheap(str, text_heap);
                        break;
                case E:	// too complex of an "e constant" loading implementation
                        asprintf(&str, "\nfldl2e\n"); // basically does (
                        push_sheap(str, text_heap);	  // 2^(log2(e)) - 1) + 1
                        asprintf(&str, "fld1\n");
                        push_sheap(str, text_heap);
                        asprintf(&str, "fld\tst1\n");
                        push_sheap(str, text_heap);
                        asprintf(&str, "fprem\n");
                        push_sheap(str, text_heap);
                        asprintf(&str, "f2xm1\n");
                        push_sheap(str, text_heap);
                        asprintf(&str, "faddp\n");
                        push_sheap(str, text_heap);
                        asprintf(&str, "fscale\n");
                        push_sheap(str, text_heap);
                        asprintf(&str, "fstp\tst1\n");
                        push_sheap(str, text_heap);
                        break; 						// FPU designers, was it too hard to create an operation for this?
                case ADD:
                        asprintf(&str, "\n\tfaddp\n");
                        push_sheap(str, text_heap);
                        break;  
                case SUB:
                        asprintf(&str, "\n\tfsubp\n");
                        push_sheap(str, text_heap);
                        break;  
                case MUL:
                        asprintf(&str, "\n\tfmulp\n");
                        push_sheap(str, text_heap);
                        break;  
                case DIV:
                        asprintf(&str, "\n\tfdivp\n");
                        push_sheap(str, text_heap);
                        break;  
                case SIN:
                        asprintf(&str, "\n\tfsin\n");
                        push_sheap(str, text_heap);
                        break;
                case COS:
                        asprintf(&str, "\n\tfcos\n");
                        push_sheap(str, text_heap);
                        break;
                case TAN:
                        asprintf(&str, "\n\tfptan\n");	// fptan returns 2 numbers on stack
                        push_sheap(str, text_heap);		// pop from stack to only return a single result
                        asprintf(&str, "\n\tfstp\tst0");
                        push_sheap(str, text_heap);
                        break;
                default:
                        //printf("[-] Something went wrong while making listing from tree");
                        break;
        }
}

void print_text_section(FILE *file) {
        char *str;
        while (str = pop_sheap(text_heap)) {
                fprintf(file, "%s", str);
        } 
}

void print_data_section(FILE *file) {
        double val;
        int i = 0;
        while (data_heap->size) {
                val = pop_dheap(data_heap);
                fprintf(file, "const%d:\tdq\t%lf\n", i + 1, val);
                i++;
        }       
}

void create_listing(char *formula, const char *func_name, int index) {    
		/*
			Creates a listing for a function
			from a string where a formula for the function
			is written in Reverse Polish Notation
		*/
        char *end, *str = calloc(20, 1);
        
        int byte_offset = 0, byte_read = 0;
        
        struct sheap data_section = {NULL, 0, 0}, text_section = {NULL, 0, 0};
        
        struct stack *stack = create_stack();

        while (
                        *(formula + byte_offset) != '\n' &&
                        *(formula + byte_offset) &&
                        sscanf(formula + byte_offset, "%s%n", str, &byte_read)) {	// reading the string iteratively
                
                push_command(str, stack);

                byte_offset += byte_read;
        }


        struct node *root = pop_stack(stack);
        
        char* temp;
        asprintf(&temp, "%s%d:\n\tpush\tebp\n\tmov\tebp, esp\n", func_name, index);
        push_sheap(temp, text_heap);

        tree_to_listing(root);

        asprintf(&temp, "\n\tleave\n\tret\n\n");
        push_sheap(temp, text_heap);

        free_stack(stack);
        free(str);
}

int main(void) {
        
        char *spec_file = getenv("SPEC_FILE");

        if (!spec_file) {
                printf("[-] SPEC_FILE not assigned\n");
				printf("[-] Listing generation failed\n");
                return 1;
        }

        FILE *input  = fopen(spec_file, "r");
        FILE *output = fopen("functions.asm", "w");

        double a, b;
        
        fscanf(input, "%lf %lf\n", &a, &b);

        
        text_heap = create_sheap();
        data_heap = create_dheap();

		if (KEY == 't') printf("[*] Generating listing for tangent method...\n");
		else if (KEY == 'c') printf("[*] Generating listing for chord method...\n");
		else { printf("[-] KEY undefined!\n"); printf("[-] Listing generation failed\n"); return 1; }

        for (int i = 1; i <= ((KEY == 't') ? 6 : 3); i++) {
                size_t n = 0;
                char *formula;
                int c = getdelim(&formula, &n, '\n', input);
                formula[c - 1] = 0; 
                
				if (i > 3) create_listing(formula, "df", i - 3);
				else create_listing(formula, "f", i);
                
				free(formula);
        }

        fprintf(output, "section .data\n");
		fprintf(output, "a:\tdq\t%lf\n", a);
		fprintf(output, "b:\tdq\t%lf\n", b);
        print_data_section(output);

        fprintf(output, "\nsection .text\n");
		fprintf(output, "\nglobal a\nglobal b\n");

		for (int i = 1; i <= 3; i++) fprintf(output, "global f%d\n", i);
        
		if (KEY == 't') {
        	for (int i = 1; i <= 3; i++) fprintf(output, "global df%d\n", i);
		}

		print_text_section(output);

        fclose(input);
        
        return 0;
}
