#ifndef KEY
#define KEY 'c'
#endif


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "functions.h"

int n = 0;          // n - iteration counter

extern double a, b;

double root_chord(double (*f)(double), double (*g)(double), double a, double b, double eps1) {
	/*
	y = (f(a) - f(b))/(a - b) * (x - a) + f(a)
	y = 0
	x = (a - b) * (-f(a)) / (f(a) - f(b)) + a
	*/
	n = 0;
	
	double deriv = ((*f)(b) - (*g)(b) - (*f)(a) + (*g)(a));
	double convex = (((*f)(a) - (*g)(a) + (*f)(b) - (*g)(b)) / 2) - ((*f)((a + b) / 2) - (*g)((a + b) / 2)); 
	double c;
	if (deriv * convex < 0.0) {
		c = b;
		while (((*f)(c) - (*g)(c)) * ((*f)(c - eps1) - (*g)(c - eps1)) >= 0.0) {
			n++;
			c = (a - c) * ((*g)(a) - (*f)(a)) / ((*f)(a) - (*g)(a) - (*f)(c) + (*g)(c)) + a;
		}
	} else {
		c = a;
		while (((*f)(c) - (*g)(c)) * ((*f)(c + eps1) - (*g)(c + eps1)) >= 0.0) {
			n++;
			c = (b - c) * ((*g)(b) - (*f)(b)) / ((*f)(b) - (*g)(b) - (*f)(c) + (*g)(c)) + b;
		}
	}
	return c;
}

double root_tangent(double (*f)(double), double (*df)(double), double (*g)(double), double (*dg)(double), double a, double b, double eps1) {
    n = 0;
    double deriv = ((*f)(a) - (*g)(a)) * ((*f)(b) - (*g)(b));
    double convex = (((*f)(a) - (*g)(a)) - ((*f)(b) - (*g)(b))) / 2 - (((*f)((a + b) / 2) - (*g)((a + b) / 2)));

    double c;
    if (deriv * convex > 0.0) {
        c = b;

        while (((*f)(c) - (*g)(c)) * ((*f)(c - eps1) - (*g)(c - eps1)) > 0) { 
            n++;

            c -= ((*f)(c) - (*g)(c)) / ((*df)(c) - (*dg)(c));
        }
        
        return c;
    } else {
        c = a;

        while (((*f)(c) - (*g)(c)) * ((*f)(c + eps1) - (*g)(c + eps1)) > 0) { 
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
    double radius = (b - a) / 4.0;
    int m = 2;

    double *arr_old = malloc(sizeof(double) * 3);
    double *arr_new = malloc(sizeof(double) * 5);

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


double f4(double x) {
    return x * x;
}

int main(int argc, char **argv) {
    //printf("%d : %lf\n", n, root_tangent(&f1, &df1, &f2, &df2, 1.0, 2.0, 0.0001)); 
    //printf("%d : %lf\n", n, root_chord(&f1, &f2, 1.0, 2.0, 0.0001));
	//printf("integral = %lf\n", integral(&f4, 0.0, 4.0, 0.01));
    
	if (KEY == 't') {
		printf("using tangent method\n");
	} else if (KEY == 'c') {
		printf("using chord method\n");
	} else {
		printf("[-]KEY not recognized %c", KEY);
	}
	return 0;
}
