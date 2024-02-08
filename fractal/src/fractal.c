#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complexlib.h"

//struct to depict the real and imaginary numbers
typedef struct {
    double real;
    double imag;
} complex;

//struct for the old and new roots of the Newton Fractal sequence
typedef struct {
    complex n;
    complex o;
}Z;

//calculating the real part of the function (following the same steps in function funim so there will be minimal comments there)
double funre(Z z, double *coef, int degree) {
    int k = 0;
    double sum = 0;
    complex s, upd;
    while (degree - k > 0) {
        s.real = 0;
        s.imag = 0;
        upd.real = z.o.real;
        upd.imag = z.o.imag;
        if (coef[degree - k] == 0) {
            k ++;
            continue;
        }
        for (int i = 2; i <= degree - k; i++) {
            s.real = remul(upd.real, upd.imag, z.o.real, z.o.imag);
            s.imag = immul(upd.real, upd.imag, z.o.real, z.o.imag);
            upd.real = s.real;
            upd.imag = s.imag;
        }
        upd.real *= coef[degree - k];
        upd.imag *= coef[degree - k];
        sum += upd.real;
        k++;
    }
    sum += coef[0];
    return sum;
}

//calculating the real part of the derivative function
double derfunre(Z z, double *coef, int degree) {
    double *dercoef = malloc(degree * sizeof(double));
    for (int i = degree; i >= 1; i--) {
        dercoef[i - 1] = i * coef[i];
    }
    int k = 0;
    degree--;
    double sum = 0;
    complex s, upd;
    while (degree - k > 0) {
        s.real = 0;
        s.imag = 0;
        upd.real = z.o.real;
        upd.imag = z.o.imag;
        for (int i = 2; i <= degree - k; i++) {
            s.real = remul(upd.real, upd.imag, z.o.real, z.o.imag);
            s.imag = immul(upd.real, upd.imag, z.o.real, z.o.imag);
            upd.real = s.real;
            upd.imag = s.imag;
        }
        if (dercoef[degree - k] != 0) {
            upd.real *= dercoef[degree - k];
            upd.imag *= dercoef[degree - k];
            sum += upd.real;
        }
        k++;
    }
    sum += dercoef[0];
    return sum;
}

//calculating the imaginary part of the function
double funim(Z z, double *coef, int degree) {
    int k = 0;
    double sum = 0;
    complex s, upd;
    while (degree - k > 0) {
        s.real = 0;
        s.imag = 0;
        upd.real = z.o.real;
        upd.imag = z.o.imag;
        if (coef[degree - k] == 0) {
            k ++;
            continue;
        }
        for (int i = 2; i <= degree - k; i++) {
            s.real = remul(upd.real, upd.imag, z.o.real, z.o.imag);
            s.imag = immul(upd.real, upd.imag, z.o.real, z.o.imag);
            upd.real = s.real;
            upd.imag = s.imag;
        }
        upd.real *= coef[degree - k];
        upd.imag *= coef[degree - k];
        sum += upd.imag;
        k++;
    }
    return sum;
}

//calculating the imaginary part of the derivative function
double derfunim(Z z, double *coef, int degree) {
    double *dercoef = malloc(degree * sizeof(double));
    for (int i = degree; i >= 1; i--) {
        dercoef[i - 1] = i * coef[i];
    }
    int k = 0;
    degree--;
    double sum = 0;
    complex s, upd;
    while (degree - k > 0) {
        s.real = 0;
        s.imag = 0;
        upd.real = z.o.real;
        upd.imag = z.o.imag;
        for (int i = 2; i <= degree - k; i++) {
            s.real = remul(upd.real, upd.imag, z.o.real, z.o.imag);
            s.imag = immul(upd.real, upd.imag, z.o.real, z.o.imag);
            upd.real = s.real;
            upd.imag = s.imag;
        }
        if (dercoef[degree - k] != 0) {
            upd.real *= dercoef[degree - k];
            upd.imag *= dercoef[degree - k];
            sum += upd.imag;
        }
        k++;
    }
    return sum;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Wrong input number. Normal input is of type ./fractal filename\n");
        exit(1);
    } 
    FILE *comp = fopen(argv[1], "r");
    if (comp == NULL) {
        fprintf(stderr, "file could not be opened\n");
        exit(1);
    }
    int degree;
    if (!(fscanf(comp, "%d", &degree))) exit(1);
    if (degree > 10) exit(1);
    double *coef = malloc((degree + 1) * sizeof(double));
    for (int i = 0; i <= degree; i ++) {
        if(!(fscanf(comp, "%lf", &coef[i]))) exit(1);
    }
    complex min, max;
    if (!(fscanf(comp, "%lf %lf %lf %lf", &min.real, &min.imag, &max.real, &max.imag))) exit(1); //getting the edges of the real and the imaginary part in the function
    double step;
    if (!(fscanf(comp, "%lf", &step))) exit(1); //getting the "depth" that the newton-raphson method searches for roots
    Z z;
    int counter = 0; //counter for total complex numbers that have run through the check
    int count = 0; //counter for total times every complex number stays in the loop
    for (double i = min.real; i < max.real; i+=step) {  
        for (double j = min.imag; j < max.imag; j+=step) {
            count = 0;
            z.n.real = i;
            z.n.imag = j;
            do {
                z.o.real = z.n.real;
                z.o.imag = z.n.imag;
                if((derfunre(z, coef, degree) == 0 && derfunim(z, coef, degree) == 0)) break;
                z.n.real -= rediv(funre(z, coef, degree), funim(z, coef, degree), derfunre(z, coef, degree), derfunim(z, coef, degree));
                z.n.imag -= imdiv(funre(z, coef, degree), funim(z, coef, degree), derfunre(z, coef, degree), derfunim(z, coef, degree));
                count ++;
            } while (imabs(z.o.real, z.o.imag, z.n.real, z.n.imag) >= 10e-6 && count < 1000);
            counter++;
            if (counter == 10e6) break;
            if (count != 1000) {
                if((derfunre(z, coef, degree) == 0.0 && derfunim(z, coef, degree) == 0.0)) {
                    printf("nan\t");
                } else {
                    if (z.n.real >= 0 && z.n.imag >= 0) printf("+%.2lf+%.2lfi\t", z.n.real, z.n.imag);
                    else if (z.n.real >= 0) printf("+%.2lf%.2lfi\t", z.n.real, z.n.imag);
                    else if (z.n.imag >= 0) printf("%.2lf+%.2lfi\t", z.n.real, z.n.imag);
                    else printf("%.2lf%.2lfi\t", z.n.real, z.n.imag);
                }
            } else printf("incomplete\t");
        }
        printf("\n");
        if (counter == 10e6) break;
    }
    fclose(comp);
    return 0;
}