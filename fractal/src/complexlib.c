#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complexlib.h"

double remul(double a, double b) {
    return (pow(a, 2)) - (pow(b, 2));
}

double immul(double a, double b) {
    return (2 * a * b);
}

double rediv(double a, double b, double c, double d) {
    return (a * c + b * d) / (pow(c, 2) + pow(d, 2));
}

double imdiv(double a, double b, double c, double d){
    return (b * c - a * d) / (pow(c, 2) + pow(d, 2));
}

double imabs(double a, double b, double c, double d) {
    return sqrt(pow(a, 2) + pow(b, 2)) - sqrt(pow(c, 2) + pow(d, 2));
}