#include <stdio.h>

double f(double x);

int main(void) {
	double x;
	scanf("%lf", &x);
	printf("f: %lf\n", f(x));
	return 0;
}
