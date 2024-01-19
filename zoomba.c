#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {
    int dim, poszoo_x, poszoo_y, trash_x, trash_y;
    if (!(fscanf(stdin, "%d", &dim))) exit(1); //getting the dimensions
    if (!(fscanf(stdin, "%d", &poszoo_x))) exit(1);
    if (!(fscanf(stdin, "%d", &poszoo_y))) exit(1);
    if (!(fscanf(stdin, "%d", &trash_x))) exit(1);
    if (!(fscanf(stdin, "%d", &trash_y))) exit(1);
    if (dim > 10000) exit(1); //checking if the dimensions exceed the allowed boundaries 
    if (poszoo_x >= dim || poszoo_y >= dim || trash_x >= dim || trash_y >= dim) exit(1); //checking if the position of the machine or the trash is out of bounds
    if (poszoo_x < 0 || poszoo_y < 0 || trash_x < 0 || trash_y < 0) exit(1);
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
    if (room[poszoo_x][poszoo_y] == 1 || room[trash_x][trash_y] == 1) exit(1); //checking if the machine or the trash is on top of an obstacle
    for (int i = 0; i < dim; i++) { //freeing the two-dimensional array of the room
        free(room[i]);
    }
    free(room);
    return 0;
}