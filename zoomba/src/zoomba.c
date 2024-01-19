#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct position {
    int x;
    int y;
};

int main(void) {
    int dim;
    struct position zoomba;
    struct position trash;
    if (!(fscanf(stdin, "%d", &dim))) exit(1); //getting the dimensions
    if (!(fscanf(stdin, "%d", &zoomba.x))) exit(1);
    if (!(fscanf(stdin, "%d", &zoomba.y))) exit(1);
    if (!(fscanf(stdin, "%d", &trash.x))) exit(1);
    if (!(fscanf(stdin, "%d", &trash.y))) exit(1);
    if (dim > 10000) exit(1); //checking if the dimensions exceed the allowed boundaries 
    if (zoomba.x >= dim || zoomba.y >= dim || trash.x >= dim || trash.y >= dim) exit(1); //checking if the position of the machine or the trash is out of bounds
    if (zoomba.x < 0 || zoomba.y < 0 || trash.x < 0 || trash.y < 0) exit(1);
    char **room = malloc(dim * sizeof(char*));
    if (room == NULL) exit(1);
        for (int i = 0; i < dim; i++) {
            room[i] = malloc(dim * sizeof(char));
            if (room[i] == NULL) exit(1);
        }
    for (int i = 0; i < dim; i++) {
        int read = fread(room[i], 1, dim, stdin);
        if (read != dim) exit(1);    
    }
    if (feof(stdin) == EOF) exit(1);
    if (room[zoomba.x][zoomba.x] == 1 || room[trash.x][trash.y] == 1) exit(1); //checking if the machine or the trash is on top of an obstacle
    for (int i = 0; i < dim; i++) { //freeing the two-dimensional array of the room
        free(room[i]);
    }
    free(room);
    return 0;
}