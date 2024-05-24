#ifndef KEY
#define KEY 'c'
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "functions.h"
#include "calculation.h"

int comp(const void *op1, const void *op2) {
	double a = *(double *)op1;
	double b = *(double *)op2;
	return (a > b) ? 1 : (a < b) ? -1 : 0; 
}

int is_equal(double a, double b, double eps) {	// needed this function, because listing functions 
	return fabs(a - b) < 2 * eps;				// calculate different numbers even if they intersect in the point
}

int main(int argc, char **argv) {

	int b_write_int = 0, b_write_iter = 0, b_write_help = 0, b_test_int = 0, b_test_area = 0;

	double eps1 = 0.001, eps2 = 0.001, int_test_val = 0.0, area_test_val = 0.0;
	double area_test_a = 0.0, area_test_b = 0.0, int_test_a = 0.0, int_test_b = 0.0;

	//double (*func_arr[])(double) = {f1, f2, f3};
	int fa, fb;

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
		} else if (!strcmp(argv[i], "-t")) {
				b_test_int = 1;

				int fid = -1;
				
				if (!sscanf(argv[++i], "f%d", &fid) || fid == -1) {
					printf("[-] Unknown function \"%s\"\n", argv[i]);
					return 1;
				}

				fa = fid - 1;

				if (!sscanf(argv[++i], "f%d", &fid) || fid == -1) {
					printf("[-] Unknown function \"%s\"\n", argv[i]);
					return 1;
				}

				fb = fid - 1;

				if (!sscanf(argv[++i], "%lf", &int_test_a)) {
					printf("[-] Unknown value\"%s\"\n", argv[i]);
					return 1;
				}
				if (!sscanf(argv[++i], "%lf", &int_test_b)) {
					printf("[-] Unknown value\"%s\"\n", argv[i]);
					return 1;
				}

				if (!sscanf(argv[++i], "%lf", &int_test_val)) {
					printf("[-] Unknown value\"%s\"\n", argv[i]);
					return 1;
				}
		} else if (!strcmp(argv[i], "-ti")) {
				b_test_area= 1;

				int fid = -1;
				
				if (!sscanf(argv[++i], "f%d", &fid) || fid == -1) {
					printf("[-] Unknown function \"%s\"\n", argv[i]);
					return 1;
				}

				fa = fid - 1;

				if (!sscanf(argv[++i], "%lf", &area_test_a)) {
					printf("[-] Unknown value\"%s\"\n", argv[i]);
					return 1;
				}
				if (!sscanf(argv[++i], "%lf", &area_test_b)) {
					printf("[-] Unknown value\"%s\"\n", argv[i]);
					return 1;
				}

				if (!sscanf(argv[++i], "%lf", &area_test_val)) {
					printf("[-] Unknown value\"%s\"\n", argv[i]);
					return 1;
				}

		} else {
			printf("[-] Flag \"%s\" not recognized\n", argv[i]);
			return 1;
		}
	}

	double f12, f23, f13;

	unsigned n12 = 0, n23 = 0, n13 = 0;

	if (b_test_int) {
		printf("[*] Testing functions \"f%d\" and \"f%d\"...\n", fa + 1, fb + 1);
		
		double ipoint;
		if (KEY == 't') {
			ipoint = root_tangent(func_arr[fa], func_arr[fa + num], func_arr[fb], func_arr[fb + num], int_test_a, int_test_b, eps1);
			if (b_write_int) printf("[+] Intersection at: %lf\n", ipoint);
		} else if (KEY == 'c') {
			ipoint = root_chord(func_arr[fa], func_arr[fb], int_test_a, int_test_b, eps1);
			if (b_write_int) printf("[+] Intersection at: %lf\n", ipoint);
		}
		if (b_write_iter) printf("[+] Calculated in %u iterations\n", n);

		if (is_equal(ipoint, int_test_val, eps1)) {
			printf("[+] OK!\n");
			return 0;	
		} else {
			printf("[-] ERR :\n\t\tExpected value:\t%lf\n\t\tResult value:\t%lf\n", int_test_val, ipoint);
			return 0;
		}
	}

	if (b_test_area) {
		printf("[*] Testing function \"f%d\"...\n", fa + 1);
		
		double area;
		
		area = integral(func_arr[fa], area_test_a, area_test_b, eps2);

		if (b_write_int) printf("[+] Area: %lf\n", area);
		
		if (b_write_iter) printf("[+] Calculated in %u iterations\n", n);

		if (is_equal(area, area_test_val, eps2)) {
			printf("[+] OK!\n");
			return 0;	
		} else {
			printf("[-] ERR :\n\t\tExpected value:\t%lf\n\t\tResult value:\t%lf\n", area_test_val, area);
			return 0;
		}
	}
    
	if (b_write_help) {
		printf( "Usage:\n\tcintegrator [ options ]\n\n"
				"When compiling the project make sure to add an environment variable called SPEC_FILE\n"
				"and specify a file to take the function file.\n\n"
				"Function file format:\n"
				"\t<N>\n\t<A> <B>\n\t<functions>\n\t[derivatives]\n\n"
				"Where A and B are interval limits of intersection and integral calculation\n"
				"<functions> is N functions represented in Reverse Polish Notation\n"
				"[derivative] is N derivativs of the functions written in Reverse Polish Notation\n"
				"(derivatives are only necessary when using the TANGENTIAL method of intersection calculation)\n\n"
				"Options:\n"
				"\t\t-help\t-h\t\t\tPrint this help message\n\n"
				"\t\t-i\t\t\t\tPrint points of intersection of functions\n\n"
				"\t\t-n\t\t\t\tPrint amount of iterations needed to calculate intersections and areas\n\n"
				"\t\t-intersection-accuracy=<eps>\tUse eps as calculation error for intersection calculation\n"
				"\t\t\t\t\t\tgeneraly use a negative power of ten (eps = 0.001 by default)\n"
				"\t\t-area-accuracy=<eps>\t\tUse eps for as calculation error for area calculation (eps = 0.001 by default)\n\n"
				"\t\t-t <Fa> <Fb> <A> <B> <val>\tTest functions <Fa> and <Fb> for intersection in interval [A, B] with expected value <val>\n\n"
				"\t\t-ti <F> <A> <B> <val>\t\tTest function <F> for integral calculation in interval [A, B] with expected value <val>\n\n"
				"MAKE Options:\n\t\tkey =\n"
				"\t\t\tt\t\t\tUse the TANGENTIAL method of calculating intersections (Must add derivatives to function file)\n"
				"\t\t\tc\t\t\tUse the CHORD method of claculating intersections (Derivatives are unnecessary)\n\n");
		return 0;
	}

	if (KEY == 't') {
		printf("[*] Using tangent method\n");
        printf("calculating f12: ");
		f12 = root_tangent(func_arr[0], func_arr[0 + num], func_arr[1], func_arr[1 + num], a, b, eps1);
        printf("%lf\n", f12);
	   	n12 = n;
        printf("calculating f13: ");
		f13 = root_tangent(func_arr[0], func_arr[0 + num], func_arr[2], func_arr[2 + num], a, b, eps1);
        printf("%lf\n", f13);
	   	n13 = n;
        printf("calculating f23: ");
		f23 = root_tangent(func_arr[1], func_arr[1 + num], func_arr[2], func_arr[2 + num], a, b, eps1);
        printf("%lf\n", f23);
	   	n23 = n;
	} else if (KEY == 'c') {
		printf("[*] Using chord method\n");
		f12 = root_chord(func_arr[0], func_arr[1], a, b, eps1);
	   	n12 = n;
		f13 = root_chord(func_arr[0], func_arr[2], a, b, eps1);
	   	n13 = n;
		f23 = root_chord(func_arr[1], func_arr[2], a, b, eps1);
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

	if (is_equal(func_arr[0](roots[0]), func_arr[1](roots[0]), eps1)) {	// calculating area by conditionally getting the order of integrals
		area += ((func_arr[0](roots[1]) > func_arr[1](roots[1])) ? 1 : -1) * (integral(func_arr[0], roots[0], roots[1], eps2) - integral(func_arr[1], roots[0], roots[1], eps2));

		if (is_equal(func_arr[0](roots[2]), func_arr[2](roots[2]), eps1)) {
			area += ((func_arr[0](roots[1]) > func_arr[2](roots[1])) ? 1 : -1) * (integral(func_arr[0], roots[1], roots[2], eps2) - integral(func_arr[2], roots[1], roots[2], eps2));
		} else {
			area += ((func_arr[1](roots[1]) > func_arr[2](roots[1])) ? 1 : -1) * (integral(func_arr[1], roots[1], roots[2], eps2) - integral(func_arr[2], roots[1], roots[2], eps2));
		}
	} else if (is_equal(func_arr[0](roots[0]), func_arr[2](roots[0]), eps1)) {
		area += ((func_arr[0](roots[1]) > func_arr[2](roots[1])) ? 1 : -1) * (integral(func_arr[0], roots[0], roots[1], eps2) - integral(func_arr[2], roots[0], roots[1], eps2));

		if (is_equal(func_arr[0](roots[2]), func_arr[1](roots[2]), eps1)) {
			area += ((func_arr[0](roots[1]) > func_arr[1](roots[1])) ? 1 : -1) * (integral(func_arr[0], roots[1], roots[2], eps2) - integral(func_arr[1], roots[1], roots[2], eps2));
		} else {
			area += ((func_arr[1](roots[1]) > func_arr[2](roots[1])) ? 1 : -1) * (integral(func_arr[1], roots[1], roots[2], eps2) - integral(func_arr[2], roots[1], roots[2], eps2));
		}
	} else {
		area += ((func_arr[1](roots[1]) > func_arr[2](roots[1])) ? 1 : -1) * (integral(func_arr[1], roots[0], roots[1], eps2) - integral(func_arr[2], roots[0], roots[1], eps2));

		if (is_equal(func_arr[0](roots[2]), func_arr[2](roots[2]), eps1)) {
			area += ((func_arr[0](roots[1]) > func_arr[2](roots[1])) ? 1 : -1) * (integral(func_arr[0], roots[1], roots[2], eps2) - integral(func_arr[2], roots[1], roots[2], eps2));
		} else {
			area += ((func_arr[0](roots[1]) > func_arr[1](roots[1])) ? 1 : -1) * (integral(func_arr[0], roots[1], roots[2], eps2) - integral(func_arr[1], roots[1], roots[2], eps2));
		}
	} 

	printf("[+] Area: %lf\n", area);

	return 0;
}
