extern unsigned n;          // n - iteration counter

double integral(double (*f)(double), double a, double b, double eps2);

double root_tangent(double (*f)(double), double (*df)(double), double (*g)(double), double (*dg)(double), double a, double b, double eps1);

double root_chord(double (*f)(double), double (*g)(double), double a, double b, double eps1);
