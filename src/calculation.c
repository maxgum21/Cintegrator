#include <math.h>

unsigned n = 0;          // n - iteration counter

double integral(double (*f)(double), double a, double b, double eps2) {
    n = 1;
    int m = 10;         // initial partition including A and B
    double radius = (b - a) / (m - 1);
    double sum = (*f)(a) + (*f)(b), integral = 0;

    for (int i = 1; i < m - 1; i++) {
        double xi = a + i * radius;
        sum += 2 * (*f)(xi);            // calculate initial sum of values
    }

    do {
        n++;
        integral = sum * radius / 2;
        for (int i = 1; i < m; i++) {
            double xi = a + radius * i - radius / 2.0;
            sum += 2 * (*f)(xi);
        }

        radius /= 2.0;

        m += m - 1;
    } while (fabs(integral - sum * radius / 2) >= eps2);

    return sum * radius / 2;    
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
