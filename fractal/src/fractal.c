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
    int sum = 0;
    while (degree - k > 0) {
        int s = 0;
        for (int i = 1; i < degree - k; i++) {
            if (s == 0) s += z.now.real;
            else s *= z.now.real;
        }
        s *= coef[degree - k];
        k++;
        sum += s;
    }
    sum += coef[0];
    return sum;
}

double dfr(Z z, double *coef, int degree) {
    int l = 0;
    for (int i = degree; i >= 1; i--) {
        coef[i] = (degree - l) * coef[i];
        l++;
    }
    int k = 1;
    int sum = 0;
    while (degree - k > 0) {
        int s = 0;
        for (int i = 1; i < degree - k; i++) {
            if (s == 0) s += z.now.real;
            s *= z.now.real;
        }
        s *= coef[degree - k];
        k++;
        sum += s;
    }
    sum += coef[1];
    return sum;
}

double fi(Z z, double *coef, int degree) {
    int k = 0;
    int sum = 0;
    while (degree - k > 0) {
        int s = 0;
        for (int i = 1; i < degree - k; i++) {
            if (s == 0) s += z.now.imag;
            else s *= z.now.imag;
        }
        s *= coef[degree - k];
        k++;
        sum += s;
    }
    sum += coef[0];
    return sum;
}

double dfi(Z z, double *coef, int degree) {
    int l = 0;
    for (int i = degree; i >= 1; i--) {
        coef[i] = (degree - l) * coef[i];
        l++;
    }
    int k = 1;
    int sum = 0;
    while (degree - k > 0) {
        int s = 0;
        for (int i = 1; i < degree - k; i++) {
            if (s == 0) s += z.now.imag;
            else s *= z.now.imag;
        }
        s *= coef[degree - k];
        k++;
        sum += s;
    }
    sum += coef[1];
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