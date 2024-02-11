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

void check_colors(Z z, unsigned char **pixel_array[], int height, int width, int *roots, int *num_roots, unsigned char *colors[], int degree) {
    //perror("hello");
    z.n.real *= 100;
    z.n.imag *= 100;
    if ((*num_roots) == 0) {
        (*num_roots) ++;
        roots[0] = (int)z.n.real;
        roots[1] = (int)z.n.imag;
        pixel_array[height][width][0] = colors[0][0];
        pixel_array[height][width][1] = colors[0][1];
        pixel_array[height][width][2] = colors[0][2];
    } else {
        for (int i = 0; i < ((*num_roots) * 2); i += 2) {
            //perror("for");
            if ((int)z.n.real == roots[i] && (int)z.n.imag == roots[i + 1]) {
                //perror("loop");
                pixel_array[height][width][0] = colors[i/2][0];
                pixel_array[height][width][1] = colors[i/2][1];
                pixel_array[height][width][2] = colors[i/2][2];
                break;
            }
        }
    }
    //perror("maybe");
    if (pixel_array[height][width][0] == 0 && pixel_array[height][width][1] == 0 && pixel_array[height][width][2] == 0 && (*num_roots) < degree) {
        //perror("yes");
        (*num_roots) ++;
        //fprintf(stderr, "%d", (*num_roots));
        roots[((*num_roots) * 2) - 2] = (int)z.n.real;
        roots[((*num_roots) * 2) - 1] = (int)z.n.imag;
        //for (int i = 0; i < 8; i++) fprintf(stderr, "%d\t", roots[i]);
        pixel_array[height][width][0] = colors[(*num_roots) - 1][0];
        pixel_array[height][width][1] = colors[(*num_roots) - 1][1];
        pixel_array[height][width][2] = colors[(*num_roots) - 1][2];
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
    int width = floor((max.imag - min.imag) / step) + 1;
    int height = floor((max.real - min.real) / step) + 1;
    unsigned char ***pixel_array = malloc(height * sizeof(unsigned char**));
        if (pixel_array == NULL) exit(1);
        for (int i = 0; i < height; i++) {
            pixel_array[i] = malloc(width * sizeof(unsigned char*));
            if(pixel_array[i] == NULL) exit(1);
            for (int j = 0; j < width; j++) {
                pixel_array[i][j] = malloc(3 * sizeof(unsigned char));
                if(pixel_array[i][j] == NULL) exit(1);
            }
        }
    for (int i = 0; i < height; i ++) {
        for (int j = 0; j < width; j ++) {
            for (int a = 0; a < 3; a++) pixel_array[i][j][a] = 0;
        }
    }
    unsigned char **colors = malloc(degree * sizeof(unsigned char*));
        if (colors == NULL) exit(1);
        for (int i = 0; i < degree; i++) {
            colors[i] = malloc(3 * sizeof(unsigned char));
        }
    int k = 0;
    if (k + 1 <= degree) {
        colors[k][0] = 50;
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
    Z z;
    int *roots = malloc((degree * 2) * sizeof(int));
    int num_roots = 0;
    int count_rep = 0; //counter for total complex numbers that have run through the check
    int count_cur = 0; //counter for total times every complex number stays in the loop
    height = 0;
    width = 0;
    for (double i = min.real; i < max.real; i+=step) {  
        width = 0;
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
                    //printf("nan\t");
                    if (argc == 4) {
                        pixel_array[height][width][0] = 0;
                        pixel_array[height][width][1] = 0;
                        pixel_array[height][width][2] = 0;
                    }
                } else {
                    // if (z.n.real >= 0 && z.n.imag >= 0) printf("+%.2lf+%.2lfi\t", z.n.real, z.n.imag);
                    // else if (z.n.real >= 0) printf("+%.2lf%.2lfi\t", z.n.real, z.n.imag);
                    // else if (z.n.imag >= 0) printf("%.2lf+%.2lfi\t", z.n.real, z.n.imag);
                    // else printf("%.2lf%.2lfi\t", z.n.real, z.n.imag);
                    if (argc == 4) {
                        check_colors(z, pixel_array, height, width, roots, &num_roots, colors, degree);
                    }
                }
            } else {
                //printf("incomplete\t");
                if (argc == 4) {
                    pixel_array[height][width][0] = 255;
                    pixel_array[height][width][1] = 255;
                    pixel_array[height][width][2] = 255;
                }
            }
            width++;
        }
        //printf("\n");
        height++;
        if (count_rep == 10e6) break;
    }
    if (argc == 4) {
        FILE *out = fopen(argv[3], "w");
        if (out == NULL) {
            fprintf(stderr, "the file could neither be opened nor created\n");
            fclose(out);
            exit(1);
        }
        unsigned char *header = malloc(54 * sizeof(unsigned char));
        if (header == NULL) exit(1);
        for (int i = 0; i < 54; i++) header[i] = 0;
        header[0] = 'B';
        header[1] = 'M';
        BMPhead info;
        info.offset = 54;
        header[OFFSET] = info.offset;
        info.headsize = 40;
        header[HEADSIZE] = info.headsize;
        info.width = width;
        header[WIDTH] = info.width;
        info.height = height;
        header[HEIGHT] = info.height;
        info.palet = 1;
        header[PALET] = *(unsigned char *)&info.palet;
        info.bits = 24;
        header[BITSPERPIX] = *(unsigned char *)&info.bits;
        info.xppm = 96;
        header[XRES] = info.xppm;
        info.yppm = 96;
        header[YRES] = info.yppm;
        int pad = 4 - (int)(3 * width) % 4;
        info.total = 3 * height * width + height * pad + 54;
        header[ALLBYTES] = info.total;
        info.pixbytes = 3 * height * width + height * pad;
        header[PIXBYTES] = info.pixbytes;
        unsigned char *final = malloc((3 * height * width + height * pad) * sizeof(unsigned char));
        int final_counter = 0;
        int flag = 0;
        for (int i = 0; i < height; i ++) {
            for (int j = 0; j < width; j ++) {
                if (pad != 0 && flag == 1) {
                    if (pad == 1) {
                        final[final_counter] = 0;
                        final_counter ++;
                    } else if (pad == 2) {
                        final[final_counter] = 0;
                        final[final_counter + 1] = 0;
                        final_counter +=2;
                    } else if (pad == 3) {
                        final[final_counter] = 0;
                        final[final_counter + 1] = 0;
                        final[final_counter + 2] = 0;
                        final_counter +=3;
                    }
                }
                final[final_counter] = pixel_array[i][j][0];
                final[final_counter + 1] = pixel_array[i][j][1];
                final[final_counter + 2] = pixel_array[i][j][2];
                final_counter += 3;
                flag = 0;
            }
            flag = 1;
        }
        int read = fwrite(header, sizeof(unsigned char), 54, out);
        if (read != 54) {
            fprintf(stderr, "could not read the headers\n");
            exit(1);
        }
        read = fwrite(final, sizeof(unsigned char), 3 * height * width + height * pad, out);
        if (read != 3 * height * width + height * pad) {
            fprintf(stderr, "Something went wrong with the pixels\n");
            exit(1);
        }
    }
    fclose(comp);
    return 0;
}