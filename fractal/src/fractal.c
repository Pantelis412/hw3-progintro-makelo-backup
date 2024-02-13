#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

// struct to depict the real and imaginary numbers
typedef struct {
  double real;
  double imag;
} complex;

// struct for the old and new roots of the Newton Fractal sequence
typedef struct {
  complex n;
  complex o;
} Z;

// struct to represent all the places the header needs values (and what they
// are) in order to work
typedef struct {
  uint32_t total;  // uint32_t for 8 bit bytes
  uint32_t offset;
  uint32_t headsize;
  uint32_t width;
  uint32_t height;
  uint16_t palet;  // uint16_t for 4 bit bytes
  uint16_t bits;
  uint32_t pixbytes;
  uint32_t xppm;
  uint32_t yppm;
} BMPhead;

// calculating the real part of the function (all of the three functions
// following are the same with minimal exceptions)
double funre(Z z, double *coef, int degree) {
  int k = 0;  // counter that lowers the degree every loop for the next variable
              // of the function
  double sum = 0;  // sum for the real part if the imaginary
  complex s, upd;  // introducing two complex variables
  while (degree - k > 0) {
    s.real = 0;  // making these two variables equal to zero(anything would
                 // work, they are temporary variables)
    s.imag = 0;
    upd.real = z.o.real;  // giving these two update variables the variables
                          // passed through from main
    upd.imag = z.o.imag;
    if (coef[degree - k] == 0) {  // avoiding variables that have a coefficient
                                  // of zero as they have no impact on the sum
      k++;
      continue;
    }
    for (int i = 2; i <= degree - k;
         i++) {  // loop that starts from 2, as variables with degree 1 or less
                 // dont have anything to be multiplied by other than their
                 // coefficients
      s.real = remul(upd.real, upd.imag, z.o.real,
                     z.o.imag);  // finding the real part of the multiplication
                                 // and storing it in a temporary variable
      s.imag = immul(upd.real, upd.imag, z.o.real,
                     z.o.imag);  // this time finding the imaginary part and
                                 // storing. The reason I need the temps is
                                 // because else the newly updated value would
                                 // go into the immul() instead of the old one
      upd.real = s.real;  // updating the real variable with the value of temp
      upd.imag =
          s.imag;  // updating the imaginary variable with the value of temp
    }
    upd.real *= coef[degree - k];  // updating the real and imaginary values
                                   // with their according coefficients
    upd.imag *= coef[degree - k];
    sum += upd.real;  // adding the real part of the degree of the polynomial to
                      // the sum
    k++;
  }
  sum += coef[0];  // updating the sum with the last coefficient of the degree 0
                   // which is equal to 1
  return sum;  // returning the sum
}

// calculating the real part of the derivative function
double derfunre(Z z, double *coef, int degree) {
  double *dercoef = malloc(degree * sizeof(double));  // allocating the coefficients of the
                                 // derivative and calculating them till line 77
  for (int i = degree; i >= 1; i--) {
    dercoef[i - 1] = i * coef[i];
  }
  int k = 0;
  degree--;  // lessening the degree by 1 as derivatives are one degree lower
             // than the functions they derived from
  double sum = 0;  // from here on put in this function, the code is identical
                   // to the last function with the exception that instead of
                   // the coef[] array, dercoef[] was used
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
  if (dercoef != 0) free(dercoef);
  return sum;
}

// calculating the imaginary part of the function (the code is the same as the
// funre function but now sum calculates the imaginary part and the last
// coefficient isn't added as it's value is always real)
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
      k++;
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

// calculating the imaginary part of the derivative function (same code as
// derfunre except sum is summing up the imaginary part of the derivative and
// the last coefficient is not calculated since it is always a real number)
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
  if (dercoef != 0) free(dercoef);
  return sum;
}

// function to input the colors of the pixel array
void check_colors(Z z, unsigned char **pixel_array[], int height, int width, int *roots, int *num_roots, unsigned char *colors[], int degree) {
  z.n.real *= 100;  // multiplying both variables by 100 to bring the digit(s)
                    // and the first two decimal digits after the decimal point
  z.n.imag *= 100;
  if ((*num_roots) == 0) {           // this if statement will only be true on the first entry
    (*num_roots)++;  // cool way to permantently update a variable, works by
                     // putting the address of the variable in the function
    roots[0] = (int)z.n.real;  // saving the integer real and integer imaginary
                               // parts of the root in two separate cells
    roots[1] = (int)z.n.imag;
    pixel_array[height][width][0] =
        colors[0][0];  // asserting the first color in the pixel array
    pixel_array[height][width][1] = colors[0][1];
    pixel_array[height][width][2] = colors[0][2];
  } else {
    for (int i = 0; i < ((*num_roots) * 2);
         i += 2) {  // a for loop that scans every root (mulitplication and step
                    // by 2 is done because the parts of the roots are
                    // essentially separate due to line 169-170)
      if ((int)z.n.real == roots[i] && (int)z.n.imag == roots[i + 1]) {  // if the real and imaginary integer parts
                               // inserted have been found already, then the
                               // pixel array gets updated
        pixel_array[height][width][0] = colors[i / 2][0];  // division of i by 2 is done because the step
                               // goes up by 2 due to the separate roots so the
                               // adjacent color of the rootis at it's half
        pixel_array[height][width][1] = colors[i / 2][1];
        pixel_array[height][width][2] = colors[i / 2][2];
        break;
      }
    }
  }
  if (pixel_array[height][width][0] == 0 && pixel_array[height][width][1] == 0 && pixel_array[height][width][2] == 0 && (*num_roots) < degree) {  // checking if the root is existent or not
    (*num_roots)++;
    roots[((*num_roots) * 2) - 2] = (int)z.n.real;  // adding the new roots to the array
    roots[((*num_roots) * 2) - 1] = (int)z.n.imag;
    pixel_array[height][width][0] = colors[(*num_roots) - 1][0];  // asserting the colors of the new roots
    pixel_array[height][width][1] = colors[(*num_roots) - 1][1];
    pixel_array[height][width][2] = colors[(*num_roots) - 1][2];
  }
}

int main(int argc, char **argv) {
  if (argc != 2 && argc != 4) {  // accepting two inputs due to the nature of
                                 // the assignment, either simple newton fractal
                                 // or option for it's visualization as well
    fprintf(stderr, "Wrong input number. Normal input is of type ./fractal filename or ./fractal filename -g output.bmp\n");
    exit(1);
  }
  FILE *comp = fopen(argv[1], "r");  // opening the first argument (file) for read only,
                            // where the degree, coefficients, number ranges and
                            // the step will be taken from
  if (comp == NULL) {  // checking if the file is empty
    fprintf(stderr, "file could not be opened\n");
    exit(1);
  }
  int degree;  // introducing a variable for the degree of the polynomials
  if (!(fscanf(comp, "%d", &degree)))
    exit(1);  // scanning the degree off of the input
  if (degree > 10) exit(1);
  double *coef = malloc(
      (degree + 1) * sizeof(double));  // allocating memory for the coefficients
  for (int i = 0; i <= degree; i++) {
    if (!(fscanf(comp, "%lf", &coef[i])))
      exit(1);  // scanning each coefficient till the degree
  }
  complex min, max;  // introducing two complex numbers for the number ranges
  if (!(fscanf(comp, "%lf %lf %lf %lf", &min.real, &min.imag, &max.real, &max.imag))) exit(1);  // getting the edges of the real and the imaginary part in the
              // function
  double step;  // creating a variable for the step of each loop (the resolution
                // of the image)
  if (!(fscanf(comp, "%lf", &step))) exit(1);  // getting the "depth" that the newton-raphson method searches for
              // roots
  int width = floor((max.imag - min.imag) / step) + 1;  // creating the width of the image
  int height = floor((max.real - min.real) / step) + 1;  // creating the height of the image
  unsigned char ***pixel_array = malloc((height) * sizeof(unsigned char **));  // creating a 3 dimensional pixel
                                             // array with height width and
                                             // depth of 3 bytes for each pixel
  if (pixel_array == NULL)
    exit(1);  // checking if allocation is possible on all the dimensions
  for (int i = 0; i < height; i++) {
    pixel_array[i] = malloc((width) * sizeof(unsigned char *));
    if (pixel_array[i] == NULL) exit(1);
    for (int j = 0; j < width; j++) {
      pixel_array[i][j] = malloc(3 * sizeof(unsigned char));
      if (pixel_array[i][j] == NULL) exit(1);
    }
  }
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      for (int a = 0; a < 3; a++)
        pixel_array[i][j][a] = 0;  // turning every cell into 0 as it would otherwise possibly be
        // used uninitialized later
    }
  }
  unsigned char **colors = malloc(
      degree * sizeof(unsigned char *));  // allocating memory for the colors
  if (colors == NULL) exit(1);
  for (int i = 0; i < degree; i++) {
    colors[i] = malloc(3 * sizeof(unsigned char));
  }
  // from line 238 to 298, manually adding colors to the array, that will be
  // later passed on to the pixel array
  int k = 0;
  if (k + 1 <= degree) {
    colors[k][0] = 0;
    colors[k][1] = 255;
    colors[k][2] = 0;
    k++;
    if (k + 1 <= degree) {
      colors[k][0] = 0;
      colors[k][1] = 0;
      colors[k][2] = 255;
      k++;
      if (k + 1 <= degree) {
        colors[k][0] = 255;
        colors[k][1] = 0;
        colors[k][2] = 0;
        k++;
        if (k + 1 <= degree) {
          colors[k][0] = 255;
          colors[k][1] = 255;
          colors[k][2] = 0;
          k++;
          if (k + 1 <= degree) {
            colors[k][0] = 0;
            colors[k][1] = 255;
            colors[k][2] = 255;
            k++;
            if (k + 1 <= degree) {
              colors[k][0] = 255;
              colors[k][1] = 0;
              colors[k][2] = 255;
              k++;
              if (k + 1 <= degree) {
                colors[k][0] = 255;
                colors[k][1] = 127;
                colors[k][2] = 127;
                k++;
                if (k + 1 <= degree) {
                  colors[k][0] = 127;
                  colors[k][1] = 127;
                  colors[k][2] = 255;
                  k++;
                  if (k + 1 <= degree) {
                    colors[k][0] = 127;
                    colors[k][1] = 255;
                    colors[k][2] = 127;
                    k++;
                    if (k + 1 <= degree) {
                      colors[k][0] = 127;
                      colors[k][1] = 127;
                      colors[k][2] = 127;
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
  Z z;  // creating a "double" complex value for the old and new variable where
        // each of them have their own real and imaginary parts
  int *roots = malloc((degree * 2) * sizeof(int));  // allocating memory for the roots
  int num_roots = 0;                       // counter for the total roots
  int count_rep = 0;  // counter for total complex numbers that have run through the check
  int count_cur = 0;  // counter for total times every complex number stays in the loop
  height = 0;  // turning height and width back to 0 as they will be used to count
  width = 0;
  for (double i = min.real; i < max.real; i += step) {
    width = 0;  // readjusting the width for the next height
    for (double j = min.imag; j < max.imag; j += step) {
      count_cur = 0;
      z.n.real = i;  // giving the new variables
      z.n.imag = j;
      do {
        z.o.real = z.n.real;  // passing the new variables to the old ones as
                              // they are about to change
        z.o.imag = z.n.imag;
        if ((derfunre(z, coef, degree) == 0 && derfunim(z, coef, degree) == 0))
          break;  // checking if the derivates are zero (division by 0)
        z.n.real -=
            rediv(funre(z, coef, degree), funim(z, coef, degree),
                  derfunre(z, coef, degree),
                  derfunim(z, coef,
                           degree));  // implementing the newton raphson method
                                      // for the real and the imaginary part
        z.n.imag -= imdiv(funre(z, coef, degree), funim(z, coef, degree),
                          derfunre(z, coef, degree), derfunim(z, coef, degree));
        count_cur++;
      } while (imabs(z.o.real, z.o.imag, z.n.real, z.n.imag) >= 10e-6 &&
               count_cur <= 1000);  // checking if the conditions are met
      count_rep++;
      if (count_rep == 10e6)
        break;  // if the repetitons reach the 10^6 then the loops stop
      if (count_cur <= 1000) {
        if ((derfunre(z, coef, degree) == 0 &&
             derfunim(z, coef, degree) ==
                 0)) {  // if the derivatives are zero then no root could be
                        // calculated
          printf("nan\t");
          if (argc == 4) {  // if the user chose 4 arguments then assign the
                            // color black to the array
            pixel_array[height][width][0] = 0;
            pixel_array[height][width][1] = 0;
            pixel_array[height][width][2] = 0;
          }
        } else {
          if (z.n.real >= 0 && z.n.imag >= 0)
            printf("+%.2lf+%.2lfi\t", z.n.real, z.n.imag);  // from line 335 to 338, printing out the roots
                               // and putting the right signs
          else if (z.n.real >= 0)
            printf("+%.2lf%.2lfi\t", z.n.real, z.n.imag);
          else if (z.n.imag >= 0)
            printf("%.2lf+%.2lfi\t", z.n.real, z.n.imag);
          else
            printf("%.2lf%.2lfi\t", z.n.real, z.n.imag);
          if (argc == 4) {  // enter the function if the user picks the visualization
            check_colors(z, pixel_array, height, width, roots, &num_roots, colors, degree);
          }
        }
      } else {  // if count of repetitions then stop the loop and print
                // incomplete
        printf("incomplete\t");
        if (argc == 4) {  // if visualization is picked, assign the color white
                          // to the array
          pixel_array[height][width][0] = 255;
          pixel_array[height][width][1] = 255;
          pixel_array[height][width][2] = 255;
        }
      }
      width++;  // adjust the width by 1
    }
    printf("\n");  // print a new line for the roots
    height++;      // adjust the height by one
    if (count_rep == 10e6) break;
  }
  if (argc == 4) {                    // from here till the end of the code
    FILE *out = fopen(argv[3], "w");  // open the bmp file for writing
    if (out == NULL) {  // if there is a problem with the opening exit the programm
      fprintf(stderr, "the file could neither be opened nor created\n");
      fclose(out);
      exit(1);
    }
    unsigned char *header = malloc(54 * sizeof(unsigned char));  // allocating memory for the header file of the bmp
    if (header == NULL) exit(1);
    for (int i = 0; i < 54; i++)
    header[i] = 0;  // assigning each header cell to 0
    header[0] = 'B';  // first two bytes
    header[1] = 'M';
    BMPhead info;  // creating a struct variable for all the different points of
                   // the header
    info.offset = 54;  // offset ends after 54 bytes from the starts so that's
                       // the value im assigning it
    header[OFFSET] = info.offset;  // inserting the offset into the array
    info.headsize = 40;  // adjusting the header size that marks the start of
                         // the pixel array
    header[HEADSIZE] = info.headsize;  // inserting the size in the array
    info.width = width;  // getting the width that we already have created
    header[WIDTH] = info.width & 0xff;  // since width and height are integers, we assign the first byte
               // (the least significant one) to the header at the position,
               // then move up 8 bits to the next byte etc. This helps with bmp
               // pictures whose height and width exceed 255
    header[WIDTH + 1] = (info.width >> 8) & 0xff;
    header[WIDTH + 2] = (info.width >> 16) & 0xff;
    header[WIDTH + 3] = (info.width >> 24) & 0xff;
    info.height = height;
    header[HEIGHT] = info.height & 0xff;
    header[HEIGHT + 1] = (info.height >> 8) & 0xff;
    header[HEIGHT + 2] = (info.height >> 16) & 0xff;
    header[HEIGHT + 3] = (info.height >> 24) & 0xff;
    info.palet = 1;  // inly using one palet of colors
    header[PALET] = *(unsigned char *)&info.palet;
    info.bits = 24;  // this is the number of bits per pixer and since 8 bits =
                     // 1 byte then 24 / 8 = 3 bytes in each pixel
    header[BITSPERPIX] = *(unsigned char *)&info.bits;
    info.xppm = 96;  // this is the standard value of pixels per meter in
                     // windows, both for the x and y axis
    header[XRES] = info.xppm;
    info.yppm = 96;
    header[YRES] = info.yppm;
    int pad = 4 - (int)(3 * width) % 4;  // introducing the padding of the picture
    int hex = 0, temp;             // introducing two temporary values
    info.total = 3 * height * width + height * pad + 54;
    if (info.total > 16777216) {  // if info.total is greater than 16^6 then you divide it
                     // and put it at the first byte, then lower info.total by
                     // 16^6 times the amount of times it was divisible by it,
                     // then proceed to do the same for every 16^(6-2k) till
                     // line 435
      temp = info.total / 16777216;
      header[ALLBYTES + hex] = temp;
      hex++;
      info.total -= 16777216 * temp;
    }
    if (info.total > 65536) {
      temp = info.total / 65536;
      header[ALLBYTES + hex] = temp;
      hex++;
      info.total -= 65536 * temp;
    }
    if (info.total > 256) {
      temp = info.total / 256;
      header[ALLBYTES + hex] = temp;
      hex++;
      info.total -= 256 * temp;
    }
    header[ALLBYTES + hex] = info.total;
    hex = 0;
    info.pixbytes = 3 * height * width + height * pad;
    if (info.pixbytes > 16777216) {
      temp = info.pixbytes / 16777216;
      header[PIXBYTES + hex] = temp;
      hex++;
      info.pixbytes -= 16777216 * temp;
    }
    if (info.pixbytes > 65536) {
      temp = info.pixbytes / 65536;
      header[PIXBYTES + hex] = temp;
      hex++;
      info.pixbytes -= 65536 * temp;
    }
    if (info.pixbytes > 256) {
      temp = info.pixbytes / 256;
      header[PIXBYTES + hex] = temp;
      hex++;
      info.pixbytes -= 256 * temp;
    }
    header[PIXBYTES + hex] = info.pixbytes;
    hex = 0;
    unsigned char *final = malloc((3 * height * width + height * pad) * sizeof(unsigned char));  // creating an array the size of PIXBYTES to store
                              // each pixel and it's bytes in 1 dimension
    int final_counter = 0;         // counter for the passing of bytes into the final array
    int flag = 0;  // variable that acts like a boolean for the padding
    for (int i = height - 1; i >= 0; i--) {  // in little endian the height starts from the bottom left
                 // instead of top left
      for (int j = 0; j < width; j++) {
        if (pad != 0 && flag == 1) {  // if there is padding and the boolean value is TRUE
                          // then from line 442 - 456 we adjust the padding and
                          // update the variables accordingly
          if (pad == 1) {
            final[final_counter] = 0;
            final_counter++;
          } else if (pad == 2) {
            final[final_counter] = 0;
            final[final_counter + 1] = 0;
            final_counter += 2;
          } else if (pad == 3) {
            final[final_counter] = 0;
            final[final_counter + 1] = 0;
            final[final_counter + 2] = 0;
            final_counter += 3;
          }
        }
        final[final_counter] = pixel_array[i][j][0];  // putting each pixel's byte in the final array
        final[final_counter + 1] = pixel_array[i][j][1];
        final[final_counter + 2] = pixel_array[i][j][2];
        final_counter += 3;
        flag = 0;  // updating the boolean value
      }
      flag = 1;  // updating the boolean value
    }
    int read = fwrite(header, sizeof(unsigned char), 54, out);  // using fwrite to put the header into the bmp
    if (read != 54) {        // checking if the fwrite faultered or not
      fprintf(stderr, "could not read the headers\n");
      exit(1);
    }
    read = fwrite(final, sizeof(unsigned char), 3 * height * width + height * pad, out);  // using fwrite to put all the bytes in the pixel array of
                      // the bmp for the final image
    if (read != 3 * height * width + height * pad) {  // checking if the fread faultered or not
      fprintf(stderr, "Something went wrong with the pixels\n");
      exit(1);
    }
    free(header);
    free(final);
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        free(pixel_array[i][j]);
      }
      free(pixel_array[i]);
    }
    free(pixel_array);
    fclose(out);  // closing the file we opened
  }
  for (int i = 0; i < degree; i++) free(colors[i]);
  free(colors);
  free(coef);
  free(roots);
  fclose(comp);  // closing the file we opened
  return 0;
}