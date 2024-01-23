#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int dim;
int DirR[] = {-1, 0, 1, 0};//direction row
int DirC[] = {0, 1, 0, -1};//direction column

typedef struct tree{
    char key;
    tree * up;
    tree * down;
    tree * right;
    tree * left;
    tree * previous;
    char data;
}tree;

int existence(char **visited, int h, int v) {// horizontal, vertical
    if (h < 0 || v < 0 || h >= dim || v >= dim) return 0;
    else if(visited[h][v])return 0;
    else return 1;
}

void find_path(char **visited,int RowZ, int ColZ){
    tree * root;
    char key=visited[RowZ][ColZ];
    char direction;
    root =NULL;
    root=(tree*) malloc(sizeof(tree));
        if (root=NULL){
            perror("Not available memory\n");
            exit(1);
        }
    root->up = root->down = root->left = root->right =NULL;
    root->key=key;
    root->previous=NULL;
    root->data='\0';

    for (int i = 0; i < 4; i++) {
        int adjx = RowZ + DirR[i];
        int adjy = ColZ + DirC[i];
        if (existence(visited, adjx, adjy)) {
           
        if(i==0)direction='U';
        if(i==1)direction='R';
        if(i==2)direction='D';
        if(i==3)direction='L';
        }
    }
    root = add_node(root, key, direction);
    
}

tree* add_node(tree *p, char key, char direction){
    if(p->up==NULL)
    {
        p->previous=p;
        p=p->up;
        p=(tree*) malloc(sizeof(tree));
        if (p=NULL){
            perror("Not available memory\n");
            exit(1);
        }
        p->up = p->down = p->left = p->right =NULL;
        p->key=key;
    
    }
    else{
        
    }
    return p;
}


int main(void) {
    int poszoo_x, poszoo_y, trash_x, trash_y;
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
    if  (room[poszoo_x][poszoo_y] == room[trash_x][trash_y]){
        printf("No steps needed. Zoomba is on the trash");
        return 0;
    }
   
    char **visited = malloc(dim * sizeof(char*)); //introducing a 2-dimensional array that marks if a cell has been visited or not
    for (int i = 0; i < dim; i++) {
        visited[i] = malloc(dim * sizeof(char));
        for (int j = 0; j < dim; j++) {
            visited[i][j] = room[i][j];
        }
    }
    visited[trash_x][trash_y] = 2;//mark the trash as 2
    visited[poszoo_x][poszoo_y] = 1;//mark zoomba as 1

    find_path(visited,poszoo_x,poszoo_y);





    for (int i = 0; i < dim; i++) { //freeing the two-dimensional array of the room
        free(visited[i]);
    }
    free(visited);
    for (int i = 0; i < dim; i++) { //freeing the two-dimensional array of the room
        free(room[i]);
    }
    free(room);
    return 0;
}