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
    for (double i = min.imag; i <= max.imag; i+=step) {  
        z.now.imag = i;
        for (double j = min.real; j <= max.real; j+=step) {
            int count = 0;
            z.now.real = j;
            // do {
            //     count ++;
            // } while (imabs(z.now.imag, z.now.real) >= 10e-6 && count < 1000);
            counter++;
            if (counter == 10e6) break;
            if (count == 1000) {
                printf("incomplete\t");
                continue;
            }
            printf("%.2lf %.2lfi\t", z.now.real, z.now.imag);
        }
        printf("\n");
        if (counter == 10e6) break;
    }
    fclose(comp);
    return 0;
}