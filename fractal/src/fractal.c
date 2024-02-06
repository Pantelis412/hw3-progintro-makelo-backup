#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complexlib.h"

//struct to depict the real and imaginary numbers
typedef struct {
    double real;
    double imag;
} complex;

typedef struct {
    complex next;
    complex now;
}Z;

double fr(Z z, double *coef, int degree) {
    int k = 0;
    complex sum;
    sum.real = 0;
    complex s;
    while (degree - k > 0) {
        s.real = 0;
        s.imag = 0;
        for (int i = 1; i <= degree - k; i++) {
            if (s.real == 0 || s.imag == 0) {
                s.real += remul(z.next.real, z.next.imag, z.next.real, z.next.imag);
                s.imag += immul(z.next.real, z.next.imag, z.next.real, z.next.imag);
            } else {
                s.real *= remul(s.real, s.imag, z.next.real, z.next.imag);
                s.imag *= immul(s.real, s.imag, z.next.real, z.next.imag);
            }
        }
        if (degree - k == 1) {
            s.real += add(s.real, z.now.real);
            s.imag += add(s.imag, z.now.imag);
        }
        if (coef[degree - k] != 0) s.real *= coef[degree - k];
        k++;
        sum.real += s.real;
    }
    sum.real += coef[0];
    return sum.real;
}

double dfr(Z z, double *coef, int degree) {
    for (int i = degree; i >= 1; i--) {
        coef[i] *= i;
    }
    int k = 1;
    complex sum;
    sum.real = 0;
    complex s;
     while (degree - k > 1) {
        s.real = 0;
        s.imag = 0;
       for (int i = 1; i <= degree - k; i++) {
            if (s.real == 0 || s.imag == 0) {
                s.real += remul(z.next.real, z.next.imag, z.next.real, z.next.imag);
                s.imag += immul(z.next.real, z.next.imag, z.next.real, z.next.imag);
            } else {
                s.real *= remul(s.real, s.imag, z.next.real, z.next.imag);
                s.imag *= immul(s.real, s.imag, z.next.real, z.next.imag);
            }
        }
        if (degree - k == 1) {
            s.real += add(s.real, z.now.real);
            s.imag += add(s.imag, z.now.imag);
        }
        if (coef[degree - k + 1] != 0) s.real *= coef[degree - k + 1];
        k++;
        sum.real += s.real;
    }
    sum.real += coef[1];
    return sum.real;
}

double fi(Z z, double *coef, int degree) {
    int k = 0;
    complex sum;
    sum.imag = 0;
    complex s;
     while (degree - k > 0) {
        s.real = 0;
        s.imag = 0;
        for (int i = 1; i <= degree - k; i++) {
            if (s.real == 0 || s.imag == 0) {
                s.real += remul(z.next.real, z.next.imag, z.next.real, z.next.imag);
                s.imag += immul(z.next.real, z.next.imag, z.next.real, z.next.imag);
            } else {
                s.real *= remul(s.real, s.imag, z.next.real, z.next.imag);
                s.imag *= immul(s.real, s.imag, z.next.real, z.next.imag);
            }
        }
        if (degree - k == 1) {
            s.real += add(s.real, z.now.real);
            s.imag += add(s.imag, z.now.imag);
        }
        if (coef[degree - k] != 0) s.imag *= coef[degree - k];
        k++;
        sum.imag += s.imag;
    }
    return sum.imag;
}

double dfi(Z z, double *coef, int degree) {
    for (int i = degree; i >= 1; i--) {
        coef[i] *= i;
    }
    int k = 1;
    complex sum;
    sum.imag = 0;
    complex s;
    while (degree - k > 0) {
        s.real = 0;
        s.imag = 0;
        for (int i = 1; i <= degree - k; i++) {
            if (s.real == 0 || s.imag == 0) {
                s.real += remul(z.next.real, z.next.imag, z.next.real, z.next.imag);
                s.imag += immul(z.next.real, z.next.imag, z.next.real, z.next.imag);
            } else {
                s.real *= remul(s.real, s.imag, z.next.real, z.next.imag);
                s.imag *= immul(s.real, s.imag, z.next.real, z.next.imag);
            }
        }
        if (degree - k == 1) {
            s.real += add(s.real, z.now.real);
            s.imag += add(s.imag, z.now.imag);
        }
        if (coef[degree - k + 1] != 0) s.imag *= coef[degree - k + 1];
        k++;
        sum.imag += s.imag;
    }
    return sum.imag;
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
    if (!(fscanf(comp, "%lf %lf %lf %lf", &min.real, &min.imag, &max.real, &max.imag))) exit(1);
    double step;
    if (!(fscanf(comp, "%lf", &step))) exit(1);
    Z z;
    int counter = 0;
    int count = 0;
    for (double i = min.imag; i <= max.imag; i+=step) {  
        z.now.imag = i;
        for (double j = min.real; j <= max.real; j+=step) {
            count = 0;
            z.now.real = j;
            do {
                z.next.real = z.now.real;
                z.next.imag = z.now.imag;
                if (dfr(z, coef, degree) != 0 || dfi(z, coef, degree) != 0) {
                z.now.real = z.now.real - rediv(fr(z, coef, degree), dfr(z, coef, degree), fi(z, coef, degree), dfi(z, coef, degree));
                z.now.imag = z.now.imag - imdiv(fi(z, coef, degree), dfi(z, coef, degree), fi(z, coef, degree), dfi(z, coef, degree));
                } else break;
                count ++;
            } while (imabs(z.now.imag, z.now.real, z.next.imag, z.next.real) >= 10e-6 && count < 1000);
            counter++;
            if (counter == 10e6) break;
            if (count == 1000) {
                printf("incomplete\t");
                continue;
            }
            if (dfr(z, coef, degree) != 0 || dfi(z, coef, degree) != 0) {
                printf("%.2lf %.2lfi\t", z.next.real, z.next.imag);
            } else printf("nan\t");
        }
        printf("\n");
        if (counter == 10e6) break;
    }
    fclose(comp);
    return 0;
}