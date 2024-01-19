#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct node {
    int dir;
    struct tree *left;
    struct tree *right;
}tree;

typedef struct {
    int x;
    int y;
}position;

void search(int dim, char **room, position *zoomba, position *trash) {

}

int main(void) {
    int dim;
    position zoomba;
    position trash;
    if (!(fscanf(stdin, "%d", &dim))) exit(1); //getting the dimensions
    fflush(stdin);
    if (!(fscanf(stdin, "%d", &zoomba.x))) exit(1);
    fflush(stdin);
    if (!(fscanf(stdin, "%d", &zoomba.y))) exit(1);
    fflush(stdin);
    if (!(fscanf(stdin, "%d", &trash.x))) exit(1);
    fflush(stdin);
    if (!(fscanf(stdin, "%d", &trash.y))) exit(1);
    fflush(stdin);
    if (dim > 10000) exit(1); //checking if the dimensions exceed the allowed boundaries 
    if (zoomba.x >= dim || zoomba.y >= dim || trash.x >= dim || trash.y >= dim) exit(1); //checking if the position of the machine and the trash is out of bounds
    if (zoomba.x < 0 || zoomba.y < 0 || trash.x < 0 || trash.y < 0) exit(1);
    char **room = malloc(dim * sizeof(char*)); //creating a two-dimensional array
    if (room == NULL) exit(1);
        for (int i = 0; i < dim; i++) {
            room[i] = malloc(dim * sizeof(char));
            if (room[i] == NULL) exit(1);
        }
    char *size = malloc((dim * dim + 1) * sizeof(char));
    int read = fread(size, 1, dim*dim, stdin);
    if (read != dim*dim) exit(1);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            room[i][j] = size[i];
        }
    }
    if (size[dim * dim] != '\0') exit(1);
    if (room[zoomba.x][zoomba.x] == 1 || room[trash.x][trash.y] == 1) exit(1); //checking if the machine or the trash is on top of an obstacle
    search(dim, room, &zoomba, &trash); //accessing the function
    for (int i = 0; i < dim; i++) { //freeing the two-dimensional array of the room
        free(room[i]);
    }
    free(room);
    return 0;
}