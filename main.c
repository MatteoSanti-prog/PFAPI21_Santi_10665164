#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//structure of a graph
typedef struct {
    unsigned int order;//it represents the number of 'arrival' of the graph
    unsigned int path;//it represents the sum of all minimum spanning trees
    struct Matrix *next;//pointer to the next element of the list (it's ordered from minimum path to the maximum one)
} Matrix;
//element of a list of graphs
typedef struct Matrix GraphInList;
//pointer to an element of the list
typedef GraphInList *Pointer;

void initializeMatrix(int * matrix, int d){//function in order to initialize matrix: its parameters are the returned matrix and its dimension
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

int main() {
    int d, k;//d=number of nodes, k=length of the ranking
    char command[15];//array used to store the command
    int* matrix;//use malloc in order to initialize the matrix because i don't know its dimension at compile time
    Pointer ranking;//ranking ordered by minimum path
    printf("Insert d and k values:\n");
    scanf("%d %d", &d, &k);
    //debugging part below
    /*
    printf("Size of variable d is: %d bytes\n", sizeof(d));
    printf("Size of variable k is: %d bytes\n", sizeof(k));
    printf("%d %d\n", d, k);
     */
    printf("Insert a specific command:\n");
    while(scanf("%s", command)!=0){
        matrix=malloc((d*d)*sizeof(int));
        if(strcmp(command,"AggiungiGrafo")==0){
            printf("Write matrix of the graph:\n");
            //initialization of the matrix
            initializeMatrix(matrix, d);
        }
        else if(strcmp(command,"TopK")==0){
            //get ranking
            printf("Here there's the top k ranking\n");
        }
        free(matrix);
        printf("Insert a specific command:\n");
    }
    return 0;
}
