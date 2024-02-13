#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complexlib.h"

//function that returns the real part of complex multiplications
double remul(double a, double b, double c, double d) {
    return (a * c - b * d);
}

//function that returns the imaginary part of complex multiplications
double immul(double a, double b, double c, double d) {
    return (a * d + b * c);
}

//function that returns the real part of complex divisions
double rediv(double a, double b, double c, double d) {
    return ((a * c + b * d) / (c * c + d * d));
}

//function that returns the imaginary part of complex divisions
double imdiv(double a, double b, double c, double d){
    return ((b * c - a * d) / (c * c + d * d));
}

//function that returns the absolute value of substraction between two complex numbers
double imabs(double a, double b, double c, double d) {
    return (fabs(sqrt(a * a + b * b) - sqrt(c * c + d * d)));
}