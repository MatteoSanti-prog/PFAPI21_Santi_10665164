#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define INF 1844674400000000000
//structure of a graph
typedef struct {
    unsigned int order;//it represents the number of 'arrival' of the graph
    unsigned long long int path;//it represents the sum of all minimum spanning trees
    struct Matrix *next;//pointer to the next element of the list (it's ordered from minimum path to the maximum one)
} Matrix;

//pointer to an element of the list
typedef Matrix *Pointer;

//this function initializes matrix: its parameters are the returned matrix and its dimension
void initializeMatrix(unsigned int * matrix, unsigned int d){
    int count1, count2;//counters used in cycle for
    for(count1=0;count1<d;count1++){
        for(count2=0;count2<d-1;count2++){
            if(scanf("%d,", &matrix[count2+(count1*d)])==EOF){
                exit(1);
            }
        }
        if(scanf("%d", &matrix[count2+(count1*d)])==EOF){
            exit(1);
        }
    }
}


//this function initializes the ranking of graphs
void initializeRanking(Pointer *ranking){
    *ranking=NULL;
}

//this function inserts a new graph in base of its minimum path's length
void insertInOrder(Pointer *ranking, unsigned int order, unsigned long long int path){
    Matrix *punt, *currentPunt, *previousPunt;
    previousPunt=NULL;
    currentPunt=*ranking;
    while(currentPunt!=NULL && path>currentPunt->path){
        previousPunt=currentPunt;
        currentPunt=(Matrix*)currentPunt->next;
    }
    while(currentPunt!=NULL && path==currentPunt->path && order>currentPunt->order){
        previousPunt=currentPunt;
        currentPunt=(Matrix*)currentPunt->next;
    }
    punt=malloc(sizeof(Matrix));
    punt->path=path;
    punt->next= (struct Matrix *) currentPunt;
    punt->order=order;
    if(previousPunt!=NULL){
        previousPunt->next= (struct Matrix *) punt;
    }
    else{
        *ranking=punt;
    }
}


//this function finds the node with minimum distance from node 0
unsigned int nodeWithMinimumDistance(unsigned long long const int *distances, unsigned int d, const int *boolean){
    int i;
    unsigned long long int min=INF;
    int node=0;
    for(i=0;i<d;i++){
        if (distances[i] <= min && boolean[i] == 0) {
            min = distances[i];
            node = i;
        }
    }
    return node;
}
//this function checks if the remaining nodes in the graph have infinite distance from node 0
int emptyList(const int *boolean, unsigned int d){
    for(int i=0;i<d;i++){
        if(boolean[i]==0 /*&& distances[i]<CONST*/){
            return 0;
        }
    }
    return 1;
}

//this function returns the sum of all minimum paths and it implements Dijkstra's algorithm
unsigned long long int graphCost(unsigned const int* matrix, unsigned int d,unsigned long long int* distances,int* boolean){
    //initialization part
    unsigned long long int sum=0;//sum of all minimum paths
    unsigned int chosen;//chosen node
    int j;//generic node
    unsigned long long int totalDistance;//total distance between a generic node and node 0

    for(j=1;j<d;j++){
        distances[j]=INF;
        boolean[j]=0;
    }
    distances[0]=0;
    boolean[0]=0;
    //main part of the algorithm
    while(emptyList(boolean,d)==0){
        chosen=nodeWithMinimumDistance(distances,d,boolean);//node with minimum distance from node 0
        boolean[chosen]=1;
        if(distances[chosen]==INF){
            break;
        }
        for(j=0;j<d;j++){//here we are considering each element of the matrix's row associated to the chosen node starting from 1 because we don't care about node 0
            if(matrix[d*chosen+j]!=0 && chosen!=j){//here i consider only the node that are reachable from the chosen node
                totalDistance=distances[chosen]+matrix[d*chosen+j];//here i update the distance between the initial node and a generic node neighbour j
                if(totalDistance<distances[j]){//here i update the distance only if it is better than the previous one
                    distances[j]=totalDistance;
                }
            }
        }
    }
    //
    //this part returns the sum of all minimum distances
    for(j=1;j<d;j++){
        if(distances[j]==INF){
            sum+=0;
        }
        else{
            sum+=distances[j];
        }
    }
    //free(distances);
    //free(boolean);
    //free(predecessors);
    //printf("Somma percorsi minimi e: %d\n", sum);
    return sum;
}

//this function displays the current ranking
void printRanking(Pointer *ranking, unsigned int k){
    if(ranking!=NULL){
        int counter=0;
        Matrix* punt=*ranking;
        if(punt!=NULL && counter<k){
            printf("%d", punt->order);
            punt=(Matrix *)punt->next;
            counter++;
            while(punt!=NULL && counter<k){
                printf(" %d", punt->order);
                punt=(Matrix *)punt->next;
                counter++;
            }
        }
        printf("\n");
    }
}

void freeMem(Pointer *ranking){
    Pointer punt;
    while(*ranking!=NULL){
        punt=*ranking;
        *ranking= (Pointer) (*ranking)->next;
        free(punt);
    }
}

int main() {
    unsigned int counter=0;//number of graphs read before the current one
    unsigned int d, k;//d=number of nodes, k=length of the ranking
    char command[15];//array used to store the command
    unsigned int* matrix;//use malloc in order to initialize the matrix because i don't know its dimension at compile time
    unsigned long long int* distances;//array that constantly saves the minimum distances of nodes from node 0
    int* boolean;//boolean in order to check which node has not been considered yet (0=false, 1=true)
    Pointer ranking;//ranking ordered by minimum path
    //printf("Insert d and k values:\n");
    if(scanf("%d %d", &d, &k)==EOF){
        exit(1);
    }
    initializeRanking(&ranking);
    matrix=malloc((d*d)*sizeof(unsigned int));
    distances=malloc(sizeof(unsigned long long int)*d);
    boolean=malloc(sizeof(int)*d);
    //printf("Insert a specific command:\n");
    while(scanf("%s", command)!=EOF){
        if(strcmp(command,"AggiungiGrafo")==0){
            if(d!=0) {
                //printf("Write matrix of the graph:\n");
                initializeMatrix(matrix, d);//initialization of the matrix
                //here there will be a function in order to calculate the sum of minimum paths
                insertInOrder(&ranking, counter, graphCost(matrix, d,distances,boolean));
                counter++;
            }
        }
        else if(strcmp(command,"TopK")==0){
            //get ranking
            //printf("Here there's the top k ranking\n");
            printRanking(&ranking, k);
        }
        //printf("Insert a specific command:\n");
    }
    free(boolean);
    free(distances);
    free(matrix);
    freeMem(&ranking);
    return 0;
}
