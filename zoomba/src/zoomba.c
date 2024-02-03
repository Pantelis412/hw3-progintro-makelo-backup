#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>

int dim;
int DirR[] = {-1, 0, 1, 0};//direction row
int DirC[] = {0, 1, 0, -1};//direction column

typedef struct tree{
    char key;
    int row;
    int collumn;
    struct tree * up;
    struct tree * down;
    struct tree * right;
    struct tree * left;
    struct tree * previous;
}tree;

typedef struct queue{
    tree * pointer;
    struct queue *next;
}queue;

int existence(char **visited, int h, int v) {// horizontal, vertical
    if (h < 0 || v < 0 || h >= dim || v >= dim) return 0;
    else if(visited[h][v]=='1')return 0;
    else return 1;
}
void add_node(tree *p, int U, int R, int D, int L, int row, int collumn, char **visited, queue **tail){
    tree *previous;
    previous=p;
    if(U==1)
    {   
        p=p->up;
        p=(tree*) malloc(sizeof(tree));
        if (p==NULL){
            perror("Not available memory\n");
            exit(1);
        }
        p->up = p->down = p->left = p->right =NULL;
        p->previous=previous;
        p->row=row-1;
        p->collumn=collumn;
        p->key=visited[p->row][p->collumn];
        visited[p->row][p->collumn]='1';
        queue * new_node;
        new_node=(queue*)malloc(sizeof(queue));
        if(new_node==NULL){
            perror("Not available memory\n");
            exit(1);
        }
        (*tail)->next=new_node;
        (*tail)=new_node;
        new_node->pointer=p;
        new_node->next=NULL;
    }
    if(R==1)
    {   
        p=p->right;
        p=(tree*) malloc(sizeof(tree));
        if (p==NULL){
            perror("Not available memory\n");
            exit(1);
        }
        p->up = p->down = p->left = p->right =NULL;
        p->previous=previous;
        p->row=row;
        p->collumn=collumn+1;
        p->key=visited[p->row][p->collumn];
        visited[p->row][p->collumn]='1';
        queue * new_node;
        new_node=(queue*)malloc(sizeof(queue));
        if(new_node==NULL){
            perror("Not available memory\n");
            exit(1);
        }
        (*tail)->next=new_node;
        (*tail)=new_node;
        new_node->pointer=p;
        new_node->next=NULL;
    }
    if(D==1)
    {   
        p=p->down;
        p=(tree*) malloc(sizeof(tree));
        if (p==NULL){
            perror("Not available memory\n");
            exit(1);
        }
        p->up = p->down = p->left = p->right =NULL;
        p->previous=previous;
        p->row=row+1;
        p->collumn=collumn;
        p->key=visited[p->row][p->collumn];
        visited[p->row][p->collumn]='1';
        queue * new_node;
        new_node=(queue*)malloc(sizeof(queue));
        if(new_node==NULL){
            perror("Not available memory\n");
            exit(1);
        }
        (*tail)->next=new_node;
        (*tail)=new_node;
        new_node->pointer=p;
        new_node->next=NULL;
    }
    if(L==1)
    {   
        p=p->left;
        p=(tree*) malloc(sizeof(tree));
        if (p==NULL){
            perror("Not available memory\n");
            exit(1);
        }
        p->up = p->down = p->left = p->right =NULL;
        p->previous=previous;
        p->row=row;
        p->collumn=collumn-1;
        p->key=visited[p->row][p->collumn];
        visited[p->row][p->collumn]='1';
        queue * new_node;
        new_node=(queue*)malloc(sizeof(queue));
        if(new_node==NULL){
            perror("Not available memory\n");
            exit(1);
        }
        (*tail)->next=new_node;
        (*tail)=new_node;
        new_node->pointer=p;
        new_node->next=NULL;
    }
}

void delete_tree(tree *p){
    if(p==NULL)
        return;
    delete_tree(p->up);
    delete_tree(p->right);
    delete_tree(p->down);
    delete_tree(p->left);
}

void find_path(char **visited,int RowZ, int ColZ){
    tree * root;
    char key=visited[RowZ][ColZ];
    root=(tree*) malloc(sizeof(tree));
        if (root==NULL){
            perror("Not available memory\n");
            exit(1);
        }
    root->up = root->down = root->left = root->right =NULL;
    root->key=key;
    root->previous=NULL;
    root->row=RowZ;
    root->collumn=ColZ;
    
    queue *head=NULL, *tail=NULL, *temp_head=NULL;
    //Firstly we fill the queue with root.
    queue * new_node;
    new_node=(queue*)malloc(sizeof(queue));
    if(new_node==NULL){
        perror("Not available memory\n");
        exit(1);
    }
    new_node->pointer=root;
    head=tail=new_node;
    new_node->next=NULL;
    int U,R,D,L;
    tree *navigator=NULL;
    while(head!=NULL){//check_add_and_pop and fill the corresponding pop in visited array with 1 if we didnt find the trash
        if(head->pointer->key=='2'){
            navigator=head->pointer;
            break;
        }
        U=R=D=L=0;
        int adjx;
        int adjy;
        for (int i = 0; i < 4; i++) {//to find what positions are free to fill the tree
        adjx = head->pointer->row;
        adjy = head->pointer->collumn;
        adjx = head->pointer->row + DirR[i];
        adjy = head->pointer->collumn + DirC[i];
        if (existence(visited, adjx, adjy)) {
           
        if(i==0){ U=1;}
        if(i==1){ R=1;}
        if(i==2){ D=1;}
        if(i==3){ L=1;}
        }
        }
        add_node(head->pointer, U, R, D, L, head->pointer->row, head->pointer->collumn, visited, &tail);
        
        temp_head=head->next;
        free(head);
        head=temp_head;
    }
    if (navigator==NULL)printf("impossible\n");
    else{
        char *path=NULL;
        int counter=0;
        while(navigator!=root){
            path=realloc(path, sizeof(char)*(counter+1));
            if (path==NULL){
                perror("Not available memory\n");
                exit(1);
            }
            if((navigator->previous->collumn)-(navigator->collumn)==1)
                path[counter]='L';
            else if((navigator->previous->collumn)-(navigator->collumn)==-1)
                path[counter]='R';
            else if((navigator->previous->row)-(navigator->row)==1)
                path[counter]='U';
            else if((navigator->previous->row)-(navigator->row)==-1)
                path[counter]='D';
            counter++;
            navigator=navigator->previous;
        }
        for(int i=counter-1; i>=0; i--){
            printf("%c",path[i]);
        }
        printf("\n");
        free(path);
       

    }   
    //free the queue
    while(head!=NULL){
        temp_head=head->next;
        free(head);
        head=temp_head;
    }
    //free the tree
    delete_tree(root);
    
}




int main(void) {
    int poszoo_x, poszoo_y, trash_x, trash_y;
    if (!(fscanf(stdin, "%d", &dim))) exit(1); //getting the dimensions
    if (!(fscanf(stdin, "%d", &poszoo_x))) exit(1);
    if (!(fscanf(stdin, "%d", &poszoo_y))) exit(1);
    if (!(fscanf(stdin, "%d", &trash_x))) exit(1);
    if (!(fscanf(stdin, "%d", &trash_y))) exit(1);
    char n;
    if (!(fscanf(stdin, "%c", &n))) exit(1);
    if (dim > 10000) exit(1); //checking if the dimensions exceed the allowed boundaries 
    if (poszoo_x >= dim || poszoo_y >= dim || trash_x >= dim || trash_y >= dim) exit(1); //checking if the position of the machine or the trash is out of bounds
    if (poszoo_x < 0 || poszoo_y < 0 || trash_x < 0 || trash_y < 0) exit(1);
    char **room = malloc(dim * sizeof(char*));
    if (room == NULL) exit(1);
        for (int i = 0; i < dim; i++) {
            room[i] = malloc(dim * sizeof(char));
            if (room[i] == NULL) exit(1);
        }
    char * temp=(char *)malloc(dim*sizeof(char));
    for (int i = 0; i < dim; i++) {
        int read = fread(room[i], 1, dim, stdin);
        if (read != dim) exit(1);
        if(!(fread(temp,1,1,stdin))) exit(1);    
    }
    if (feof(stdin) == EOF) exit(1);
    if (room[poszoo_x][poszoo_y] == '1' || room[trash_x][trash_y] == '1') exit(1); //checking if the machine or the trash is on top of an obstacle
    if  (&room[poszoo_x][poszoo_y] == &room[trash_x][trash_y]){
        printf("No steps needed. Zoomba is on the trash");
        return 0;
    }
   
    char **visited = malloc(dim * sizeof(char*)); //introducing a 2-dimensional array that marks if a cell has been visited or not
    for (int i = 0; i < dim; i++) {
        visited[i] = malloc(dim * sizeof(char));
        //strcpy(visited[i],room[i]);
        for (int j = 0; j < dim; j++) {
            visited[i][j] = room[i][j];
        }
    }
    visited[trash_x][trash_y] = '2';//mark the trash as 2
    visited[poszoo_x][poszoo_y] = '1';//mark zoomba as 1
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
