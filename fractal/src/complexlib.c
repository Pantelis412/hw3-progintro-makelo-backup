#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complexlib.h"

double remul(double a, double b, double c, double d) {
    return (a * c - b * d);
}

double immul(double a, double b, double c, double d) {
    return (a * d + b * c);
}

double rediv(double a, double b, double c, double d) {
    //printf("%.2lf %.2lf %.2lf %.2lf\t", a, b, c, d);
    return ((a * c + b * d) / (c * c + d * d));
}

double imdiv(double a, double b, double c, double d){
    return ((b * c - a * d) / (c * c + d * d));
}

double imabs(double a, double b, double c, double d) {
    return (fabs(sqrt(a * a + b * b) - sqrt(c * c + d * d)));
}