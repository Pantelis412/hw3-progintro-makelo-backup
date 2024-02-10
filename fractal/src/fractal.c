#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complexlib.h"
#define ALLBYTES 2
#define OFFSET 10
#define HEADSIZE 14
#define WIDTH 18
#define HEIGHT 22
#define PALET 26
#define BITSPERPIX 28
#define PIXBYTES 34
#define XRES 38
#define YRES 42

//struct to depict the real and imaginary numbers
typedef struct {
    double real;
    double imag;
} complex;

//struct for the old and new roots of the Newton Fractal sequence
typedef struct {
    complex n;
    complex o;
} Z;

typedef struct {
    int total;
    int offset;
    int headsize;
    int width;
    int height;
    unsigned char palet;
    unsigned char bits;
    int pixbytes;
    int xppm;
    int yppm;
} BMPhead;

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

void check_colors(Z z, unsigned char **pixel_array[], int height, int width, double *roots, int *num_roots, unsigned char *colors[]) {
    perror("hi");
    if ((*num_roots) == 0) {
        printf("%d\n", (*num_roots));
        (*num_roots) ++;
        printf("%d\n", (*num_roots));
        roots = realloc(roots, (*num_roots) * 2 * sizeof(complex));
        roots[0] = z.n.real;
        roots[1] = z.n.imag;
        pixel_array[height][width][0] = colors[0][0];
        pixel_array[height][width][1] = colors[0][1];
        pixel_array[height][width][2] = colors[0][2];
    } else {
        for (int i = 0; i <= (*num_roots) * 2; i += 2) {
            if (z.n.real == roots[i] && z.n.imag == roots[i + 1]) {
                perror("ho");
                pixel_array[height][width][0] = colors[i/2][0];
                pixel_array[height][width][1] = colors[i/2][1];
                pixel_array[height][width][2] = colors[i/2][2];
            }
        }
    }
    perror("ho");
    if (pixel_array[height][width][0] == 0 && pixel_array[height][width][1] == 0 && pixel_array[height][width][2] == 0) {
        perror("ho");
        (*num_roots) ++;
        roots = realloc(roots, (*num_roots) * 2 * sizeof(complex));
        roots[((*num_roots) * 2) - 2] = z.n.real;
        roots[((*num_roots) * 2) - 1] = z.n.imag;
        pixel_array[height][width][0] = colors[(*num_roots)][0];
        pixel_array[height][width][1] = colors[(*num_roots)][1];
        pixel_array[height][width][2] = colors[(*num_roots)][2];
    }
}

int main(int argc, char **argv) {
    if (argc != 2 && argc != 4) {
        fprintf(stderr, "Wrong input number. Normal input is of type ./fractal filename or ./fractal filename -g output.bmp\n");
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
    if (argc == 4) {
        FILE *out = fopen(argv[3], "w");
        if (out == NULL) {
            fprintf(stderr, "the file could neither be opened nor created\n");
            fclose(out);
            exit(1);
        }
        unsigned char *header = malloc(54 * sizeof(unsigned char));
        if (header == NULL) exit(1);
        for (int i = 0; i < 54; i++) header[i] = '0';
        header[0] = 'B';
        header[1] = 'M';
        BMPhead info;
        info.total = 3 * (ceil((max.real - min.real) / step) * ceil((max.imag - min.imag) / step)) + 54;
        header[ALLBYTES] = info.total;
        info.offset = 54;
        header[OFFSET] = info.offset;
        info.headsize = 40;
        header[HEADSIZE] = info.headsize;
        info.width = ceil((max.imag - min.imag) / step);
        header[WIDTH] = info.width;
        info.height = ceil((max.real - min.real) / step);
        header[HEIGHT] = info.height;
        info.palet = 1;
        header[PALET] = *(unsigned char *)&info.palet;
        info.bits = 24;
        header[BITSPERPIX] = *(unsigned char *)&info.bits;
        info.pixbytes = 3 * (ceil((max.real - min.real) / step) * ceil((max.imag - min.imag) / step));
        header[PIXBYTES] = info.pixbytes;
        info.xppm = 96;
        header[XRES] = info.xppm;
        info.yppm = 96;
        header[YRES] = info.yppm;
        // int read = fwrite(header, sizeof(unsigned char), 54, comp);
        // if (read != 54) {
        //     fprintf(stderr, "could not read the headers\n");
        //     exit(1);
        // }
    } 
    unsigned char ***pixel_array = malloc(ceil((max.real - min.real) / step) * sizeof(unsigned char**));
        if (pixel_array == NULL) exit(1);
        for (int i = 0; i < ceil((max.real - min.real) / step); i++) {
            pixel_array[i] = malloc(ceil((max.imag - min.imag) / step) * sizeof(unsigned char*));
            if(pixel_array[i] == NULL) exit(1);
            for (int j = 0; j < ceil((max.imag - min.imag) / step); j++) {
                pixel_array[i][j] = malloc(3 * sizeof(unsigned char));
                if(pixel_array[i][j] == NULL) exit(1);
            }
        }
    for (int i = 0; i < ceil((max.real - min.real) / step); i ++) {
        for (int j = 0; j < ceil((max.imag - min.imag) / step); j ++) {
            for (int a = 0; a < 3; a++) pixel_array[i][j][a] = '0';
        }
    }
    unsigned char **colors = malloc(degree * sizeof(unsigned char*));
        if (colors == NULL) exit(1);
        for (int i = 0; i < degree; i++) {
            colors[i] = malloc(3 * sizeof(unsigned char));
        }
    int k = 0;
    if (k + 1 <= degree) {
        colors[k][0] = 223;
        colors[k][1] = 255;
        colors[k][2] = 0;
        k++;
        if (k + 1 <= degree) {
            colors[k][0] = 255;
            colors[k][1] = 191;
            colors[k][2] = 0;
            k++;
            if (k + 1 <= degree) {
                colors[k][0] = 255;
                colors[k][1] = 127;
                colors[k][2] = 80;
                k++;
                if (k + 1 <= degree) {
                    colors[k][0] = 222;
                    colors[k][1] = 49;
                    colors[k][2] = 99;
                    k++;
                    if (k + 1 <= degree) {
                        colors[k][0] = 159;
                        colors[k][1] = 226;
                        colors[k][2] = 191;
                        k++;
                        if (k + 1 <= degree) {
                            colors[k][0] = 64;
                            colors[k][1] = 244;
                            colors[k][2] = 208;
                            k++;
                            if (k + 1 <= degree) {
                                colors[k][0] = 100;
                                colors[k][1] = 149;
                                colors[k][2] = 237;
                                k++;
                                if (k + 1 <= degree) {
                                    colors[k][0] = 204;
                                    colors[k][1] = 204;
                                    colors[k][2] = 255;
                                    k++;
                                    if (k + 1 <= degree) {
                                        colors[k][0] = 255;
                                        colors[k][1] = 0;
                                        colors[k][2] = 127;
                                        k++;
                                        if (k + 1 <= degree) {
                                            colors[k][0] = 25;
                                            colors[k][1] = 51;
                                            colors[k][2] = 0;
                                            k++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // for (int i = 0; i < degree; i ++) {
    //     for (int j = 0; j < degree; j ++) printf("%x\t", colors[i][j]);
    // }
    Z z;
    double *roots = NULL;
    int num_roots = 0;
    int count_rep = 0; //counter for total complex numbers that have run through the check
    int count_cur = 0; //counter for total times every complex number stays in the loop
    int height = 0;
    int width = 0;
    for (double i = min.real; i < max.real; i+=step) {  
        for (double j = min.imag; j < max.imag; j+=step) {
            count_cur = 0;
            z.n.real = i;
            z.n.imag = j;
            do {
                z.o.real = z.n.real;
                z.o.imag = z.n.imag;
                if((derfunre(z, coef, degree) == 0 && derfunim(z, coef, degree) == 0)) break;
                z.n.real -= rediv(funre(z, coef, degree), funim(z, coef, degree), derfunre(z, coef, degree), derfunim(z, coef, degree));
                z.n.imag -= imdiv(funre(z, coef, degree), funim(z, coef, degree), derfunre(z, coef, degree), derfunim(z, coef, degree));
                count_cur ++;
            } while (imabs(z.o.real, z.o.imag, z.n.real, z.n.imag) >= 10e-6 && count_cur <= 1000);
            count_rep++;
            if (count_rep == 10e6) break;
            if (count_cur <= 1000) {
                if((derfunre(z, coef, degree) == 0.0 && derfunim(z, coef, degree) == 0.0)) {
                    printf("nan\t");
                    if (argc == 4) {
                        pixel_array[height][width][0] = 0;
                        pixel_array[height][width][1] = 0;
                        pixel_array[height][width][2] = 0;
                    }
                } else {
                    if (z.n.real >= 0 && z.n.imag >= 0) printf("+%.2lf+%.2lfi\t", z.n.real, z.n.imag);
                    else if (z.n.real >= 0) printf("+%.2lf%.2lfi\t", z.n.real, z.n.imag);
                    else if (z.n.imag >= 0) printf("%.2lf+%.2lfi\t", z.n.real, z.n.imag);
                    else printf("%.2lf%.2lfi\t", z.n.real, z.n.imag);
                    if (argc == 4) {
                        check_colors(z, pixel_array, height, width, roots, &num_roots, colors);
                    }
                }
            } else {
                printf("incomplete\t");
                if (argc == 4) {
                    pixel_array[height][width][0] = 255;
                    pixel_array[height][width][1] = 255;
                    pixel_array[height][width][2] = 255;
                }
            }
            width++;
        }
        printf("\n");
        height++;
        if (count_rep == 10e6) break;
    }
    if (argc == 4) {
        int read = fwrite(pixel_array, sizeof(unsigned char), 3 * (ceil((max.real - min.real) / step) * ceil((max.imag - min.imag) / step)), comp);
        if (read != 3 * (ceil((max.real - min.real) / step) * ceil((max.imag - min.imag) / step))) {
            fprintf(stderr, "Something went wrong with the pixels\n");
            exit(1);
        }
    }
    fclose(comp);
    return 0;
}