#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CONST 10000000

//structure of a graph
typedef struct {
    unsigned int order;//it represents the number of 'arrival' of the graph
    unsigned int path;//it represents the sum of all minimum spanning trees
    struct Matrix *next;//pointer to the next element of the list (it's ordered from minimum path to the maximum one)
} Matrix;

//pointer to an element of the list
typedef Matrix *Pointer;

//list of nodes in a graph
typedef struct{
    unsigned int node;
    struct listOfNodes *next;
}listOfNodes;

typedef listOfNodes *List;

//this function initializes matrix: its parameters are the returned matrix and its dimension
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


//this function initializes the ranking of graphs
void initializeRanking(Pointer *ranking){
    *ranking=NULL;
}

//this function initializes the list of nodes in the graph
void initializeList(List *list){
    *list=NULL;
}


//this function inserts a new graph in base of its minimum path's length
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

//this function insert a node of the graph in the specified list
void insertNode(List *list, int node){
    listOfNodes *punt;
    if(*list==NULL){
        punt=malloc(sizeof(listOfNodes));
        punt->next=NULL;
        punt->node=node;
        *list=punt;
    }
    else{
        insertNode(&((*list)->next), node);
    }
}

//this function delete a node from the specified list
void deleteNode(List *list, int node){
    listOfNodes *punt;
    if(*list!=NULL){
        if((*list)->node==node){
            punt=*list;
            *list=(*list)->next;
            free(punt);
        }
        else{
            deleteNode(&((*list)->next), node);
        }
    }
}

//this function finds the node with minimum distance from node 0
int nodeWithMinimumDistance(int distances[], int d){
    int i;
    int temp=CONST;
    for(i=0;i<d;i++){
        if(distances[i]<temp){
            temp=distances[i];
        }
    }
    return temp;
}

//this function returns the sum of all minimum paths and it implements Dijkstra's algorithm
int graphCost(int* matrix, int d){

    //initialization part
    int sum=0;//sum of all minimum paths
    int chosen;//chosen node
    int j;//generic node
    int totalDistance;//total distance between a generic node and node 0
    int distances[d];//array that constantly saves the minimum distances of nodes from node 0
    int predecessors[d];//array of nodes (a generic predecessors[d] is the previous node of node d in the minimum path from node 0)
    List listOfNodes;//list of all nodes in the graph
    initializeList(&listOfNodes);
    for(j=0;j<d;j++){
        predecessors[j]=0;
        distances[j]=CONST;
        insertNode(&listOfNodes,j);
    }
    distances[0]=0;
    predecessors[0]=0;
    //
    //main part of the algorithm
    while(listOfNodes!=NULL){
        chosen=nodeWithMinimumDistance(distances,d);//node with minimum distance from node 0
        deleteNode(&listOfNodes, chosen);
        if(chosen==CONST){//other nodes will have infinite distances too
            break;
        }
        for(j=0;j<d;j++){//here we are considering each element of the matrix's row associated to the chosen node
            if(matrix[d*chosen+j]!=0){//here i consider only the node that are reachable from the chosen node
                totalDistance=distances[chosen]+matrix[d*chosen+j];//here i update the distance between the initial node and a generic node neighbour j
                if(totalDistance<distances[j]){//here i update the distance only if it is better than the previous one
                    distances[j]=totalDistance;
                    predecessors[j]=chosen;
                }
            }
        }
    }
    //
    //this part returns the sum of all minimum distances
    for(j=0;j<d;j++){
        if(distances[j]==CONST){
            sum+=0;
        }
        else{
            sum+=distances[j];
        }
    }
    return sum;
    //
}

//this function displays the current ranking
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
            insertInOrder(&ranking,counter,graphCost(&matrix, d));
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
