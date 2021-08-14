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

//pointer to an element of the graphList
typedef Matrix *Pointer;

//structure of a node
typedef struct{
    unsigned int node;//it represents the index of the node
    unsigned long long int distance;//it represents the current distance of the node from node 0
    struct Node *next;//pointer to the next element of the list
}Node;
//pointer to an element of the nodeList
typedef Node *PointerNode;

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

//this function initializes the list of all nodes
void initializeNode(PointerNode *listOfNodes){
    *listOfNodes=NULL;
}


void insertNode(PointerNode *listOfNodes, unsigned int i){
    Node *punt;
    punt=malloc(sizeof(Node));
    punt->node=i;
    if(i==0){
        punt->distance=0;
    }else{
        punt->distance=INF;
    }
    punt->next= (struct Node *) *listOfNodes;
    *listOfNodes=punt;
}

//this function inserts a new graph in base of its minimum path's length
void insertInOrder(Pointer *ranking, unsigned int order, unsigned long long int path, unsigned int k){
    unsigned int counter=0;
    Matrix *punt, *currentPunt, *previousPunt;
    previousPunt=NULL;
    currentPunt=*ranking;
    while(currentPunt!=NULL && path>currentPunt->path && counter<k){
        previousPunt=currentPunt;
        currentPunt=(Matrix*)currentPunt->next;
        counter++;
    }
    while(currentPunt!=NULL && path==currentPunt->path && order>currentPunt->order && counter<k){
        previousPunt=currentPunt;
        currentPunt=(Matrix*)currentPunt->next;
        counter++;
    }
    if(counter<k){
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
}


//this function returns the node with minimum distance which is the first of the list.

void deleteElement(PointerNode *listOfNodes){
    if(*listOfNodes!=NULL){
        Node *punt;
        punt=*listOfNodes;
        *listOfNodes= (PointerNode) (*listOfNodes)->next;
        free(punt);
    }
}

//this function checks if there are no more nodes
int emptyList(PointerNode *listOfNodes){
    if(*listOfNodes==NULL){
        return 1;
    }
    else{
        return 0;
    }
}

unsigned long long int checkDistance(PointerNode *listOfNode, unsigned int node){
    Node *punt;
    punt=*listOfNode;
    while(punt!=NULL && punt->node!=node){
        punt= (Node *) punt->next;
    }
    if(punt==NULL){
        return 0;
    }
    else{
        return punt->distance;
    }
}

void deleteNode(PointerNode *listOfNodes, unsigned int node){
    Node *punt;
    if(*listOfNodes!=NULL){
        if((*listOfNodes)->node==node){
            punt=*listOfNodes;
            *listOfNodes= (PointerNode) (*listOfNodes)->next;
            free(punt);
        }
        else{
            deleteNode((PointerNode *) &((*listOfNodes)->next), node);
        }
    }
}

void updateDistance(PointerNode *listOfNodes, unsigned int node, unsigned long long int newDistance){
    Node *punt, *currentPunt, *prevPunt;
    prevPunt=NULL;
    currentPunt=*listOfNodes;
    while(currentPunt!=NULL && newDistance>currentPunt->distance){
        prevPunt=currentPunt;
        currentPunt= (Node *) currentPunt->next;
    }
    punt=malloc(sizeof(Node));
    punt->node=node;
    punt->distance=newDistance;
    punt->next= (struct Node *) currentPunt;
    if(prevPunt!=NULL){
        prevPunt->next= (struct Node *) punt;
    }
    else{
        *listOfNodes=punt;
    }
    deleteNode((PointerNode *) &(punt->next), node);
}

unsigned int getNode(PointerNode *listOfNodes){
    return (*listOfNodes)->node;
}

unsigned long long int getDistance(PointerNode *listOfNodes){
    return (*listOfNodes)->distance;
}

//this function returns the sum of all minimum paths, and it implements Dijkstra's algorithm
unsigned long long int graphCost(unsigned const int* matrix, unsigned int d){
    PointerNode listOfNodes;
    initializeNode(&listOfNodes);
    unsigned int j;//generic node
    unsigned long long int sum=0;
    unsigned long long int distance;
    unsigned long long int temp;
    unsigned int chosen;//node with minimum distance
    for(j=0;j<d;j++){
        insertNode(&listOfNodes,d-j-1);
    }
    while(emptyList(&listOfNodes)==0){
        chosen=getNode(&listOfNodes);//node with minimum distance from node 0
        distance=getDistance(&listOfNodes);
        deleteElement(&listOfNodes);
        if(distance==INF){
            sum+=0;
            break;
        }
        else{
            sum+=distance;
        }
        if(emptyList(&listOfNodes)==0){
            for(j=0;j<d;j++){//here we are considering each element of the matrix's row associated to the chosen node starting from 1 because we don't care about node 0
                if(matrix[d*chosen+j]!=0 && chosen!=j){//here I consider only the node that are reachable from the chosen node
                    temp=checkDistance(&listOfNodes, j);
                    if(temp!=0 && distance+matrix[d*chosen+j]<temp){
                        updateDistance(&listOfNodes, j, distance+matrix[d*chosen+j]);
                    }
                }
            }
        }
    }
    while(emptyList(&listOfNodes)==0){
        deleteElement(&listOfNodes);
    }
    //printf("Somma percorsi minimi e: %llu\n", sum);
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
    Pointer ranking;//ranking ordered by minimum path
    //printf("Insert d and k values:\n");
    if(scanf("%d %d", &d, &k)==EOF){
        exit(1);
    }
    initializeRanking(&ranking);
    matrix=malloc((d*d)*sizeof(unsigned int));
    //printf("Insert a specific command:\n");
    while(scanf("%s", command)!=EOF){
        if(strcmp(command,"AggiungiGrafo")==0){
            if(d!=0) {
                //printf("Write matrix of the graph:\n");
                initializeMatrix(matrix, d);//initialization of the matrix
                //here there will be a function in order to calculate the sum of minimum paths
                insertInOrder(&ranking, counter, graphCost(matrix, d), k);
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
    //free(matrix);
    //freeMem(&ranking);
    return 0;
}
