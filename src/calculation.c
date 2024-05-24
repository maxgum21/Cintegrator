#include <stdlib.h>
#include <math.h>

unsigned n = 0;          // n - iteration counter

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

	free(arr_old);
	free(arr_new);

    return i_2n;
}

double root_tangent(double (*f)(double), double (*df)(double), double (*g)(double), double (*dg)(double), double a, double b, double eps1) {
	// Calculating f and g intersection with the tangent method
    n = 0;
    //double deriv = ((*f)(a) - (*g)(a)) * ((*f)(b) - (*g)(b));
    //double convex = (((*f)(a) - (*g)(a)) + ((*f)(b) - (*g)(b))) / 2 - (((*f)((a + b) / 2) - (*g)((a + b) / 2)));

    double deriv = (*df)(a) - (*dg)(a);
    double convex = ((*df)(a) - (*dg)(a)) * ((*df)(b) - (*dg)(b)); 

    double c;
    if (deriv * convex > 0.0) {
        c = b;

        while (
				((*f)(c) - (*g)(c)) * ((*f)(c - eps1) - (*g)(c - eps1)) > 0.0
				) { 
			if (((*f)(c) - (*g)(c)) * ((*f)(c + eps1) - (*g)(c + eps1)) <= 0.0) return c + eps1;
            n++;

            c -= ((*f)(c) - (*g)(c)) / ((*df)(c) - (*dg)(c));
        }
        
        return c;
    } else {
        c = a;

        while (
				((*f)(c) - (*g)(c)) * ((*f)(c + eps1) - (*g)(c + eps1)) > 0.0
				) { 
			if (((*f)(c) - (*g)(c)) * ((*f)(c - eps1) - (*g)(c - eps1)) <= 0.0) return c - eps1;
            n++;
            c -= ((*f)(c) - (*g)(c)) / ((*df)(c) - (*dg)(c));

        }
        
        return c;
    }
}

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
				((*f)(c) - (*g)(c)) * ((*f)(c - eps1) - (*g)(c - eps1)) > 0.0
				) {
			if (((*f)(c) - (*g)(c)) * ((*f)(c + eps1) - (*g)(c + eps1)) <= 0.0) return c + eps1;
			n++;
			c = (a - c) * ((*g)(a) - (*f)(a)) / ((*f)(a) - (*g)(a) - (*f)(c) + (*g)(c)) + a;
		}
	} else {
		c = a;
		while (
				((*f)(c) - (*g)(c)) * ((*f)(c + eps1) - (*g)(c + eps1)) > 0.0
				) {
			if (((*f)(c) - (*g)(c)) * ((*f)(c - eps1) - (*g)(c - eps1)) <= 0.0) return c - eps1;
			n++;
			c = (b - c) * ((*g)(b) - (*f)(b)) / ((*f)(b) - (*g)(b) - (*f)(c) + (*g)(c)) + b;
		}
	}
	return c;
}
