#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX 100000000

int counter;
int dim;
int DirR[] = {-1, 0, 1, 0};
int DirC[] = {0, 1, 0, -1};

struct node{
    int value;
    struct node *next;
};

typedef struct{
    struct node *front;
    struct node *rear;
    int size[MAX];
} queue;

typedef struct {
    int x;
    int y;
} Position;

// int enqueue(queue *q, ) {

// }

int existence(char **visited, int h, int v) {
    if (h < 0 || v < 0 || h >= dim || v >= dim || visited[h][v]) return 0;
    else return 1;
}

int empty(queue* q) {
  if (q->rear == NULL)
    return 1;
  else
    return 0;
}

queue *create() {
   queue *q = malloc(sizeof(queue));
   q->front = NULL;
   q->rear = NULL;
}

int print(char *choices) {
    choices = realloc(choices, (counter + 1) * sizeof(char));
    choices[counter + 1] = '\0';
    printf("%s\n", choices);
}

int direction(char **visited, int RowZ, int ColZ, char *choices, int count, Position trash) {
    for (int i = 0; i < 4; i++) {
        int adjx = RowZ + DirR[i];
        int adjy = ColZ + DirC[i];
        if (existence(visited, adjx, adjy)) {
            RowZ = adjx;
            ColZ = adjy;
            if (RowZ != trash.x && ColZ != trash.y) {
                visited[RowZ][ColZ] = 1;
                if (i == 0) choices[count] = 'U';
                else if (i == 1) choices[count] = 'R';
                else if (i == 2) choices[count] = 'D';
                else if (i == 3) choices[count] = 'L';
                count++;
                return direction(visited, RowZ, ColZ, choices, count, trash);
            } else {
                counter = count;
                return 1;
            }
        }
    }
    if (choices[count - 1] == 'U') RowZ++;
    else if (choices[count - 1] == 'R') ColZ--;
    else if (choices[count - 1] == 'D') RowZ--;
    else if (choices[count - 1] == 'L') ColZ++;
    choices[count] = '\0';
    count--;
    if (count >= 0) return direction(visited, RowZ, ColZ, choices, count, trash);
    else return 0;
}

void search(char **room, Position zoomba, Position trash) {
    char **visited = malloc(dim * sizeof(char*)); //introducing a 2-dimensional array that marks if a cell has been visited or not
    for (int i = 0; i < dim; i++) {
        visited[i] = malloc(dim * sizeof(char));
        for (int j = 0; j < dim; j++) {
            visited[i][j] = room[i][j];
        }
    }
    visited[zoomba.x][zoomba.y] = 1;
    char *choices = malloc(dim * dim * sizeof(char));
    int RowZ = zoomba.x;
    int ColZ = zoomba.y;
    queue *q = create();
    //enqueue(q, );
    int count = 0;
    //while (!empty(q)) {
    //}
    if(direction(visited, RowZ, ColZ, choices, count, trash) == 0) {
        printf("impossible\n");
        exit(1);
    }
    print(choices);
}

int main(void) {
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
    search(room, zoomba, trash); //accessing the function
    for (int i = 0; i < dim; i++) { //freeing the two-dimensional array of the room
        free(room[i]);
    }
    free(room);
    return 0;
}