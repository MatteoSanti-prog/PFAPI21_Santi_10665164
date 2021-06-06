#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//structure of a graph
typedef struct {
    unsigned int order;//it represents the number of 'arrival' of the graph
    unsigned int path;//it represents the sum of all minimum spanning trees
    struct Matrix *next;//pointer to the next element of the list (it's ordered from minimum path to the maximum one)
} Matrix;

//pointer to an element of the list
typedef Matrix *Pointer;


//function in order to initialize matrix: its parameters are the returned matrix and its dimension
void initializeMatrix(int * matrix, int d){
    int count1, count2;//counters used in cycle for
    for(count1=0;count1<d;count1++){
        for(count2=0;count2<d-1;count2++){
            scanf("%d,", &matrix[count2+(count1*d)]);
        }
        scanf("%d", &matrix[count2+(count1*d)]);
    }
    //debugging part below
    /*
    printf("Here there's the representation of your matrix:\n");
    for(count1=0;count1<d;count1++){
        for(count2=0;count2<d-1;count2++){
            printf("%d,", matrix[count2+(count1*d)]);
        }
        printf("%d\n", matrix[count2+(count1*d)]);
    }
     */
}


//function in order to initialize the ranking of graphs
void initializeRanking(Pointer *ranking){
    *ranking=NULL;
}


//function that insert a new graph in base of his minimum path's length
void insertInOrder(Pointer *ranking, int order, int path){
    Matrix *punt, *currentPunt, *previousPunt;
    previousPunt=NULL;
    currentPunt=*ranking;
    while(currentPunt!=NULL && path>currentPunt->path){
        previousPunt=currentPunt;
        currentPunt=currentPunt->next;
    }
    while(currentPunt!=NULL && path==currentPunt->path && order>currentPunt->order){
        previousPunt=currentPunt;
        currentPunt=currentPunt->next;
    }
    punt=malloc(sizeof(Matrix));
    punt->path=path;
    punt->next=currentPunt;
    punt->order=order;
    if(previousPunt!=NULL){
        previousPunt->next=punt;
    }
    else{
        *ranking=punt;
    }
}


//function in order to display the current ranking
void printRanking(Pointer *ranking, int k){
    if(ranking!=NULL){
        int counter=0;
        Matrix* punt=*ranking;
        while(punt!=NULL && counter<k){
            printf("%d ", punt->order);
            punt=punt->next;
            counter++;
        }
        printf("\n");
    }
}

int main() {
    int counter=0;//number of graphs read before the current one
    int d, k;//d=number of nodes, k=length of the ranking
    char command[15];//array used to store the command
    int* matrix;//use malloc in order to initialize the matrix because i don't know its dimension at compile time
    Pointer ranking;//ranking ordered by minimum path
    initializeRanking(&ranking);
    printf("Insert d and k values:\n");
    scanf("%d %d", &d, &k);
    //debugging part below
    /*
    printf("Size of variable d is: %d bytes\n", sizeof(d));
    printf("Size of variable k is: %d bytes\n", sizeof(k));
    printf("%d %d\n", d, k);
     */
    printf("Insert a specific command:\n");
    while(scanf("%s", command)!=EOF){
        matrix=malloc((d*d)*sizeof(int));
        if(strcmp(command,"AggiungiGrafo")==0){
            printf("Write matrix of the graph:\n");
            initializeMatrix(matrix, d);//initialization of the matrix
            //here there will be a function in order to calculate the sum of minimum paths
            insertInOrder(&ranking,counter,2);//toDo
            counter++;
        }
        else if(strcmp(command,"TopK")==0){
            //get ranking
            printf("Here there's the top k ranking\n");
            printRanking(&ranking, k);
        }
        printf("Insert a specific command:\n");
    }
    free(matrix);
    return 0;
}
