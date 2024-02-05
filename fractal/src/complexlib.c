#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"

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