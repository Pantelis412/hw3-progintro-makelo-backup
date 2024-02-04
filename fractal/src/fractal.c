#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double real;
    double imag;
} complex;

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
    double *coef = malloc((degree + 1) * sizeof(double));
    for (int i = 0; i < (degree + 1); i ++) {
        if(!(fscanf(comp, "%lf", &coef[i]))) exit(1);
    }
    complex min, max;
    if (!(fscanf(comp, "%lf %lf %lf %lf", min.real, min.imag, max.real, max.imag))) exit(1);
    fclose(comp);
    return 0;
} 