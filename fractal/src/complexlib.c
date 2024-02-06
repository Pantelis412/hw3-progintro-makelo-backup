#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"

void coef_complex(double coefficient, double *real, double *imaginary){//coefficient * complex
    *real=(*real)*coefficient;
    *imaginary=(*imaginary)*coefficient;
}
void pow_complex(double *real, double *imaginary, double power){//complex ^ power
    double static_real=(*real);
    double static_imaginary=(*imaginary);
    for (int i=0; i<power-1; i++){
        (*real)=(*real)*static_real-(*imaginary)*static_imaginary;
        (*imaginary)=(*real)*static_imaginary+(*imaginary)*static_real;
    }
}

double imadd(double a, double b, double c, double d) {
    
}

double imsub(double a, double b, double c, double d) {

}

double immul(double a, double b, double c, double d) {
    return (a * c - b * d) + (a * d + b * c);
}

double imdiv(double a, double b, double c, double d) {

}

double imabs(double a, double b) {
    return sqrt(pow(a, 2) + pow(b, 2));
}