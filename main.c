#ifndef KEY
#define KEY 'c'
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "functions.h"

unsigned n = 0;          // n - iteration counter

double root_chord(double (*f)(double), double (*g)(double), double a, double b, double eps1) {
	/*
	Calculating f and g intersection with the chord method
	y = (f(a) - f(b))/(a - b) * (x - a) + f(a)
	y = 0
	x = (a - b) * (-f(a)) / (f(a) - f(b)) + a
	*/
	n = 0;
	
	double deriv = ((*f)(b) - (*g)(b) - (*f)(a) + (*g)(a));	// calculating derivative sign
	double convex = (((*f)(a) - (*g)(a) + (*f)(b) - (*g)(b)) / 2) - ((*f)((a + b) / 2) - (*g)((a + b) / 2)); // calculating second-order derivative sign
	double c;
	if (deriv * convex < 0.0) {
		c = b;
		while (
				((*f)(c) - (*g)(c)) * ((*f)(c - eps1) - (*g)(c - eps1)) > 0.0 &&
				((*f)(c) - (*g)(c)) * ((*f)(c + eps1) - (*g)(c + eps1)) > 0.0
				) {
			n++;
			c = (a - c) * ((*g)(a) - (*f)(a)) / ((*f)(a) - (*g)(a) - (*f)(c) + (*g)(c)) + a;
		}
	} else {
		c = a;
		while (
				((*f)(c) - (*g)(c)) * ((*f)(c + eps1) - (*g)(c + eps1)) > 0.0 &&
				((*f)(c) - (*g)(c)) * ((*f)(c - eps1) - (*g)(c - eps1)) > 0.0
				) {
			n++;
			c = (b - c) * ((*g)(b) - (*f)(b)) / ((*f)(b) - (*g)(b) - (*f)(c) + (*g)(c)) + b;
		}
	}
	return c;
}

double root_tangent(double (*f)(double), double (*df)(double), double (*g)(double), double (*dg)(double), double a, double b, double eps1) {
	// Calculating f and g intersection with the tangent method
    n = 0;
    double deriv = ((*f)(a) - (*g)(a)) * ((*f)(b) - (*g)(b));
    double convex = (((*f)(a) - (*g)(a)) - ((*f)(b) - (*g)(b))) / 2 - (((*f)((a + b) / 2) - (*g)((a + b) / 2)));

    double c;
    if (deriv * convex > 0.0) {
        c = b;

        while (
				((*f)(c) - (*g)(c)) * ((*f)(c - eps1) - (*g)(c - eps1)) > 0 &&
				((*f)(c) - (*g)(c)) * ((*f)(c + eps1) - (*g)(c + eps1)) > 0
				) { 
            n++;

            c -= ((*f)(c) - (*g)(c)) / ((*df)(c) - (*dg)(c));
        }
        
        return c;
    } else {
        c = a;

        while (
				((*f)(c) - (*g)(c)) * ((*f)(c + eps1) - (*g)(c + eps1)) > 0 &&
				((*f)(c) - (*g)(c)) * ((*f)(c - eps1) - (*g)(c - eps1)) > 0
				) { 
            n++;
            c -= ((*f)(c) - (*g)(c)) / ((*df)(c) - (*dg)(c));

        }
        
        return c;
    }
}

double integral(double (*f)(double), double a, double b, double eps2) {
    n = 0;
    double i_n = 0;
    double i_2n = 0;
    double radius = (b - a) / 4.0;					// radius is the legnth from one partition point to the next
    int m = 2;

    double *arr_old = malloc(sizeof(double) * 3);	// array to store current points of interval partition
    double *arr_new = malloc(sizeof(double) * 5);	// array to store next points of partition

    arr_old[0] = (*f)(a);
    arr_old[1] = (*f)(a + 2 * radius);
    arr_old[2] = (*f)(b);

    double c;
    do {
        n++;
        i_n = i_2n;
        i_2n = 0;
        c = a;

        arr_new[0] = arr_old[0];
        arr_new[2 * m] = arr_old[m];

        for (int i = 0; i < 2 * m; i++, c += radius) {
            if (i % 2 == 0) {
                arr_new[i] = arr_old[i / 2];
                arr_new[i + 1] = (*f)(c + radius);
            } else {
                arr_new[i + 1] = arr_old[(i + 1) / 2];
            }

            i_2n += radius * (arr_new[i] + arr_new[i + 1]) / 2.0;
        }

        m <<= 1;
        radius /= 2.0;        

        free(arr_old);
        arr_old = arr_new;
        arr_new = malloc(sizeof(double) * (2 * m + 1));

    } while (fabs(i_n - i_2n) >= eps2);

    return i_2n;
}

int comp(const void *op1, const void *op2) {
	double a = *(double *)op1;
	double b = *(double *)op2;
	return (a > b) ? 1 : (a < b) ? -1 : 0; 
}

int is_equal(double a, double b, double eps) {	// needed this function, because listing functions 
	return fabs(a - b) < 2 * eps;				// calculate different numbers even if they intersect in the point
}

int main(int argc, char **argv) {

	int b_write_int = 0, b_write_iter = 0, b_write_help = 0;

	double eps1 = 0.001, eps2 = 0.001;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-help") || !strcmp(argv[i], "-h")) {
			b_write_help = 1;
		} else if (!strcmp(argv[i], "-i")) {
			b_write_int  = 1;
		} else if (!strcmp(argv[i], "-n")) {
			b_write_iter = 1;
		} else if (!strncmp(argv[i], "-intersection-accuracy=", 23)) {
	   		sscanf(argv[i], "-intersection-accuracy=%lf", &eps1);
		} else if (!strncmp(argv[i], "-area-accuracy=", 14)) {
	   		sscanf(argv[i], "-area-accuracy=%lf", &eps2);
		} else {
			printf("[-] Flag \"%s\" not recognized\n", argv[i]);
			return 1;
		}
	}

	double f12, f23, f13;

	unsigned n12 = 0, n23 = 0, n13 = 0;
    
	if (b_write_help) {
		printf( "Usage:\n\tcintegrator [ options ]\n\n"
				"When compiling the project make sure to add an environment variable called SPEC_FILE\n"
				"and specify a file to take the function file.\n\n"
				"Function file format:\n"
				"\t<A> <B>\n\t<f1>\n\t<f2>\n\t<f3>\n\t[df1]\n\t[df2]\n\t[df3]\n\n"
				"Where A and B are interval limits of intersection and integral calculation\n"
				"<f*> is a function represented in Reverse Polish Notation\n"
				"[df*] is a derivative of the f* function written in Reverse Polish Notation\n"
				"(derivatives are only necessary when using the TANGENTIAL method of intersection calculation)\n\n"
				"Options:\n"
				"\t\t-help\t-h\t\t\tPrint this help message\n"
				"\t\t-i\t\t\t\tPrint points of intersection of functions\n"
				"\t\t-n\t\t\t\tPrint amount of iterations needed to calculate intersections and areas\n"
				"\t\t-intersection-accuracy=<eps>\tUse eps as calculation error for intersection calculation\n"
				"\t\t\t\t\t\tgeneraly use a negative power of ten (eps = 0.001 by default)\n"
				"\t\t-area-accuracy=<eps>\t\tUse eps for as calculation error for area calculation (eps = 0.001 by default)\n\n"
				"MAKE Options:\n\t\t\"key\" =\n"
				"\t\t\t\'t\'\tUse the TANGENTIAL method of calculating intersections (Must add derivatives to function file)\n"
				"\t\t\t\'c\'\tUse the CHORD method of claculating intersections (Derivatives are unnecessary)\n\n");
		return 0;
	}

	if (KEY == 't') {
		printf("[*] Using tangent method\n");
		f12 = root_tangent(&f1, &df1, &f2, &df2, a, b, eps1);
	   	n12 = n;
		f13 = root_tangent(&f1, &df1, &f3, &df3, a, b, eps1);
	   	n13 = n;
		f23 = root_tangent(&f2, &df2, &f3, &df3, a, b, eps1);
	   	n23 = n;
	} else if (KEY == 'c') {
		printf("[*] Using chord method\n");
		f12 = root_chord(&f1, &f2, a, b, eps1);
	   	n12 = n;
		f13 = root_chord(&f1, &f3, a, b, eps1);
	   	n13 = n;
		f23 = root_chord(&f2, &f3, a, b, eps1);
	   	n23 = n;
	} else {
		printf("[-] KEY \"%c\" not recognized\n", KEY);
		return 1;
	}

	if (b_write_int) {
		printf("[+] F1 and F2 point of intersection: %lf\n", f12);
		printf("[+] F1 and F3 point of intersection: %lf\n", f13);
		printf("[+] F2 and F3 point of intersection: %lf\n\n", f23);
	}

	if (b_write_iter) {
		printf("[+] F1 and F2 intersection calculation took: %u iterations\n", n12);
		printf("[+] F1 and F3 intersection calculation took: %u iterations\n", n13);
		printf("[+] F2 and F3 intersection calculation took: %u iterations\n\n", n23);
	}

	double roots[3] = {f12, f13, f23};

	qsort(roots, 3, sizeof(double), &comp);

	double area = 0;

	if (is_equal(f1(roots[0]), f2(roots[0]), eps1)) {	// calculating area by conditionally getting the order of integrals
		area += ((f1(roots[1]) > f2(roots[1])) ? 1 : -1) * (integral(&f1, roots[0], roots[1], eps2) - integral(&f2, roots[0], roots[1], eps2));

		if (is_equal(f1(roots[2]), f3(roots[2]), eps1)) {
			area += ((f1(roots[1]) > f3(roots[1])) ? 1 : -1) * (integral(&f1, roots[1], roots[2], eps2) - integral(&f3, roots[1], roots[2], eps2));
		} else {
			area += ((f2(roots[1]) > f3(roots[1])) ? 1 : -1) * (integral(&f2, roots[1], roots[2], eps2) - integral(&f3, roots[1], roots[2], eps2));
		}
	} else if (is_equal(f1(roots[0]), f3(roots[0]), eps1)) {
		area += ((f1(roots[1]) > f3(roots[1])) ? 1 : -1) * (integral(&f1, roots[0], roots[1], eps2) - integral(&f3, roots[0], roots[1], eps2));

		if (is_equal(f1(roots[2]), f2(roots[2]), eps1)) {
			area += ((f1(roots[1]) > f2(roots[1])) ? 1 : -1) * (integral(&f1, roots[1], roots[2], eps2) - integral(&f2, roots[1], roots[2], eps2));
		} else {
			area += ((f2(roots[1]) > f3(roots[1])) ? 1 : -1) * (integral(&f2, roots[1], roots[2], eps2) - integral(&f3, roots[1], roots[2], eps2));
		}
	} else {
		area += ((f2(roots[1]) > f3(roots[1])) ? 1 : -1) * (integral(&f2, roots[0], roots[1], eps2) - integral(&f3, roots[0], roots[1], eps2));

		if (is_equal(f1(roots[2]), f3(roots[2]), eps1)) {
			area += ((f1(roots[1]) > f3(roots[1])) ? 1 : -1) * (integral(&f1, roots[1], roots[2], eps2) - integral(&f3, roots[1], roots[2], eps2));
		} else {
			area += ((f1(roots[1]) > f2(roots[1])) ? 1 : -1) * (integral(&f1, roots[1], roots[2], eps2) - integral(&f2, roots[1], roots[2], eps2));
		}
	} 

	printf("[+] Area: %lf\n", area);

	return 0;
}
