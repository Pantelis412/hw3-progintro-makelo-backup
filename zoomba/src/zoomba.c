#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int DirR[] = {-1, 0, 1, 0};
int DirC[] = {0, 1, 0, -1};

typedef struct {

} Queue;

struct node{
    struct node *next;
};

typedef struct {
    int x;
    int y;
} Position;

// int Existence() {
//     if () (

//     )
// }

void search(int dim, char **room, Position zoomba, Position trash) {
    char **visited = malloc(dim * sizeof(char*)); //introducing a 2-dimensional array that marks if a cell has been visited or not
    for (int i = 0; i < dim; i++) {
        visited[i] = malloc(dim * sizeof(char));
        for (int j = 0; j < dim; j++) {
            visited[i][j] = room[i][j];
        }
    }
    visited[zoomba.x][zoomba.y] = 1;
    int RowZ = zoomba.x;
    int ColZ = zoomba.y;
    int RowT = trash.x;
    int ColT = trash.y;
}

int main(void) {
    int dim;
    Position zoomba;
    Position trash;
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
    if (room[zoomba.x][zoomba.y] == 1 || room[trash.x][trash.y] == 1) exit(1); //checking if the machine or the trash is on top of an obstacle
    search(dim, room, zoomba, trash); //accessing the function
    for (int i = 0; i < dim; i++) { //freeing the two-dimensional array of the room
        free(room[i]);
    }
    free(room);
    return 0;
}